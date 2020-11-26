// dp/powerGenerating.cpp
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

using namespace std;

// 発電計画問題
// 正の整数Tが与えられ、時刻t=0,...,T−1のそれぞれについて時間 [t,t+1]に発電装置をオンにするかを決める必要がある。
// ある「連続してオンになっている時間の全体」(厳密には、(時間[l - 1, l]にオフ)||(l = 0) && (時間[r, r + 1]にオフ)||(r = T - 1) && (時間[l, r]にオン))に対して利得はg[l][r](0≤l<r≤T)である。発電計画全体の利得は「連続してオンになっている時間の全体」への利得の総和の時、これを最大化せよ
// ・1≤T≤100
// ・0≤g[i][j]≤1000

template<typename T, typename U>
T& upd_max(T& a, const U& b) {
    return a = max(a, static_cast<T>(b));
}

template<typename profit_type>
profit_type power_generating(const vector< vector<profit_type> >& v) {
    int n = v.size();
    vector<profit_type> dp(n + 1);
    for (int i = 0; i <= n; ++i) for (int j = 0; j < i; ++j) for (int k = j + 1; k < i; ++k) upd_max(dp.at(i), dp.at(j) + v.at(j).at(k));
    return dp.at(n);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    vector< vector<int> > g(t + 1, vector<int>(t + 1));
    for (int i = 0; i < t; ++i) for (int j = 1; j <= t; ++j) cin >> g.at(i).at(j);
    cout << power_generating(g) << '\n';
    return 0;
}
