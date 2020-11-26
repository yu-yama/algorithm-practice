// dp/lcs.cpp
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
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

template<typename T, typename U>
T& upd_max(T& a, const U& b) {
    return a = max(a, static_cast<T>(b));
}
template<typename T, typename U, typename Compare>
T& upd_max(T& a, const U& b, Compare comp) {
    return a = max(a, static_cast<T>(b), comp);
}
template<typename T, typename U>
T& upd_max(T& a, initializer_list<U> b) {
    return upd_max(a, max(b));
}
template<typename T, typename U, typename Compare>
T& upd_max(T& a, initializer_list<U> b, Compare comp) {
    return upd_max(a, max(b, comp), comp);
}
template<typename T, typename U>
T& upd_min(T& a, const U& b) {
    return a = min(a, static_cast<T>(b));
}
template<typename T, typename U, typename Compare>
T& upd_min(T& a, const U& b, Compare comp) {
  return a = min(a, static_cast<T>(b), comp);
}
template<typename T, typename U>
T& upd_min(T& a, initializer_list<U> b) {
    return upd_min(a, min(b));
}
template<typename T, typename U, typename Compare>
T& upd_min(T& a, initializer_list<U> b, Compare comp) {
    return upd_min(a, min(b, comp), comp);
}

// ・1≤|S|,|T|≤1000

template<typename T>
int lcs(const T& s, const T& t) {
    int n = s.size(), m = t.size();
    vector< vector<int> > dp(n + 1, vector<int>(m + 1));
    for (int i = 0; i < n; ++i) for (int j = 0; j < m; ++j) {
        if (s.at(i) == t.at(j)) upd_max(dp.at(i + 1).at(j + 1), dp.at(i).at(j) + 1);
        upd_max(dp.at(i + 1).at(j + 1), {dp.at(i).at(j + 1), dp.at(i + 1).at(j)});
    }
    return dp.at(n).at(m);
}

template<typename T>
string lcs_s(const T& s, const T& t) {
    int n = s.size(), m = t.size();
    vector< vector<T> > dp(n + 1, vector<T>(m + 1));
    auto comp = [](T i, T j) -> bool {
        return i.size() < j.size();
    };
    for (int i = 0; i < n; ++i) for (int j = 0; j < m; ++j) {
        if (s.at(i) == t.at(j)) upd_max(dp.at(i + 1).at(j + 1), dp.at(i).at(j) + s.at(i), comp);
        upd_max(dp.at(i + 1).at(j + 1), {dp.at(i).at(j + 1), dp.at(i + 1).at(j)}, comp);
    }
    return dp.at(n).at(m);
}

template<typename T>
int levenshtein(const T& s, const T& t) {
    constexpr int INF = 1 << 30;
    int n = s.size(), m = t.size();
    vector< vector<int> > dp(n + 1, vector<int>(m + 1, INF));
    dp.at(0).at(0) = 0;
    for (int i = -1; i < n; ++i) for (int j = -1; j < m; ++j) {
        if (i >= 0 && j >= 0) upd_min(dp.at(i + 1).at(j + 1), dp.at(i).at(j) + (s.at(i) != t.at(j)));
        if (i >= 0) upd_min(dp.at(i + 1).at(j + 1), dp.at(i).at(j + 1) + 1);
        if (j >= 0) upd_min(dp.at(i + 1).at(j + 1), dp.at(i + 1).at(j) + 1);
    }
    return dp.at(n).at(m);
}

// A=(a0, a1, …, am−1)とB=(b0, b1, …, bn−1)の最小コスト弾性マッチング
// c(i, j) := (ai, bj)をマッチさせたときのコスト
// ・1≤m,n≤1000
template<typename cost_type>
cost_type elastic_matching(const vector< vector<cost_type> >& c) {
    int m = c.size(), n = c.at(0).size();
    vector< vector<cost_type> > dp(m + 1, vector<cost_type>(n + 1));
    for (int i = 0; i < m; ++i) for (int j = 0; j < n; ++j) dp.at(i + 1).at(j + 1) = min({dp.at(i).at(j), dp.at(i + 1).at(j), dp.at(i).at(j + 1)}) + c.at(i).at(j);
    return dp.at(m).at(n);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s, t;
    cin >> s >> t;
    cout << lcs(s, t) << '\n';
    cout << lcs_s(s, t) << '\n';
    cout << levenshtein(s, t) << '\n';
    return 0;
}
