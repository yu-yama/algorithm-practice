// dp/knapsack.cpp
//
// Copyright 2020 yu-yama
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template<typename T, typename U>
T& upd_max(T& a, const U& b) {
    return a = max(a, static_cast<T>(b));
}

template<typename weight_type, typename value_type>
struct Item {
    weight_type weight;
    value_type value;
};

// ナップサック問題
// ・1≤n≤100
// ・1≤W≤10000
// ・weight[i],value[i]は整数
// ・1≤weight[i],value[i]≤1000

template<typename weight_type, typename value_type>
value_type knapsack1(int capacity, const vector< Item<weight_type, value_type> >& v) {
    vector<value_type> dp(capacity + 1, 0);
    for (const auto& i : v) for (weight_type j = capacity - i.weight; j >= 0; --j) upd_max(dp.at(j + i.weight), dp.at(j) + i.value);
    return dp.at(capacity);
}

template<typename weight_type, typename value_type>
value_type knapsack2(int capacity, const vector< Item<weight_type, value_type> >& v) {
    int n = v.size();
    vector< vector<value_type> > dp(n + 1, vector<value_type>(capacity + 1, 0));
    for (int i = 0; i < n; ++i) for (weight_type j = 0; j <= capacity; ++j) {
        if (j + v.at(i).weight <= capacity) upd_max(dp.at(i + 1).at(j + v.at(i).weight), dp.at(i).at(j) + v.at(i).value);
        upd_max(dp.at(i + 1).at(j), dp.at(i).at(j));
    }
    return dp.at(n).at(capacity);
}

// 個数制限なしナップサック

template<typename weight_type, typename value_type>
value_type multiple_knapsack(int capacity, const vector< Item<weight_type, value_type> >& v) {
    vector<value_type> dp(capacity + 1, 0);
    for (const auto& i : v) for (weight_type j = 0; j <= capacity - i.weight; ++j) upd_max(dp.at(j + i.weight), dp.at(j) + i.value);
    return dp.at(capacity);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, w;
    cin >> n >> w;
    vector< Item<int, int> > v(n);
    for (auto&& i : v) cin >> i.weight >> i.value;
    cout << knapsack1(w, v) << '\n';
    cout << knapsack2(w, v) << '\n';
    cout << multiple_knapsack(w, v) << '\n';
    return 0;
}
