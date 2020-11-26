// dp/lis.cpp
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
#include <limits>
#include <algorithm>
#include <vector>

using namespace std;

// 最長部分増加列問題

template<typename number_type>
int lis1(const vector<number_type>& v) {
    constexpr number_type INF = numeric_limits<number_type>::max();
    int n = v.size();
    vector<number_type> dp(n, INF);
    for (const auto& i : v) *lower_bound(dp.begin(), dp.end(), i) = i;
    return lower_bound(dp.begin(), dp.end(), INF) - dp.begin();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<int> a(n);
    for (auto&& i : a) cin >> i;
    cout << lis1(a) << '\n';
    return 0;
}
