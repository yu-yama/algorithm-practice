// dp/subsetSum.cpp
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
#include <bitset>
#include <limits>
#include <vector>
#include <algorithm>

using namespace std;

using mod_type = long long;

template<typename T, typename U>
constexpr T extended_euclidean(const T& a, const T& b, U& x, U& y) noexcept {
    if (!b) {
        x = 1, y = 0;
        return a;
    }
    T t = a / b, r = extended_euclidean(b, a - b * t, y, x);
    y -= x * t;
    return r;
}

template<mod_type m>
struct Fp {
    using number_type = long long;
    constexpr static number_type max_num = numeric_limits<number_type>::max();
private:
    number_type n;
    constexpr void check_negative() noexcept {
        if (n < 0) n += m;
    }
public:
    constexpr Fp(number_type nn = 0) noexcept : n(nn % m) {
        check_negative();
    }
    constexpr mod_type mod() const noexcept {
        return m;
    }
    template<typename T> constexpr operator T() const {
        return T(n);
    }
    constexpr Fp operator+() const noexcept {
        return *this;
    }
    constexpr Fp operator-() const noexcept {
        return n ? m - n : 0;
    }
    constexpr Fp& operator++() noexcept {
        return *this += 1;
    }
    constexpr Fp operator++(int) noexcept {
        Fp t(*this);
        ++(*this);
        return t;
    }
    constexpr Fp& operator--() noexcept {
        return *this -= 1;
    }
    constexpr Fp operator--(int) noexcept {
        Fp t(*this);
        --(*this);
        return t;
    }
    constexpr Fp& operator+=(const Fp& a) noexcept {
        if ((n += a.n) >= m) n -= m;
        return *this;
    }
    constexpr Fp& operator-=(const Fp& a) noexcept {
        return *this += -a;
    }
    constexpr Fp& operator*=(const Fp& a) noexcept {
        if (!a) return *this = 0;
        if (n <= max_num / a.n) {
            (n *= a.n) %= m;
        } else {
            Fp t(*this);
            (t += t) *= Fp(a.n >> 1);
            if (a.n & 1) *this += t;
            else *this = t;
        }
        return *this;
    }
    constexpr Fp inv() const noexcept {
        Fp u, v;
        extended_euclidean<long long, Fp>(n, m, u, v);
        return u;
    }
    constexpr Fp& operator/=(const Fp& a) noexcept {
        *this *= a.inv();
        check_negative();
        return *this;
    }
    constexpr Fp operator+(const Fp& a) const noexcept {
        return Fp(*this) += a;
    }
    template<typename T> friend constexpr Fp operator+(const T& a, const Fp& b) noexcept {
        return b + a;
    }
    constexpr Fp operator-(const Fp& a) const noexcept {
        return Fp(*this) -= a;
    }
    template<typename T> friend constexpr Fp operator-(const T& a, const Fp& b) noexcept {
        return -(b - a);
    }
    constexpr Fp operator*(const Fp& a) const noexcept {
        return Fp(*this) *= a;
    }
    template<typename T> friend constexpr Fp operator*(const T& a, const Fp& b) noexcept {
        return b * a;
    }
    constexpr Fp operator/(const Fp& a) const noexcept {
        return Fp(*this) /= a;
    }
    template<typename T> friend constexpr Fp operator/(const T& a, const Fp& b) noexcept {
        return (b / a).inv();
    }
    constexpr bool operator==(const Fp& a) const noexcept {
        return n == a.n;
    }
    constexpr bool operator!=(const Fp& a) const noexcept {
        return n != a.n;
    }
    constexpr Fp pow(const number_type& a) const noexcept {
        if (!a) return Fp(1);
        if (a < 0) return inv().pow(-a);
        Fp t = pow(a >> 1);
        t *= t;
        if (a & 1) t *= *this;
        return t;
    }
    friend constexpr istream& operator>>(istream& s, Fp& a) {
        return s >> a.n;
    }
    friend constexpr ostream& operator<<(ostream& s, const Fp& a) {
        return s << a.n;
    }
};

template<typename T, typename U>
T& upd_max(T& a, const U& b) {
    return a = max(a, static_cast<T>(b));
}
template<typename T, typename U>
T& upd_min(T& a, const U& b) {
    return a = min(a, static_cast<T>(b));
}

// ・1≤n≤100
// ・1≤A≤10000
// ・1≤a[i]≤1000

// 部分和問題
// O(n * MAX(A))
template<typename number_type, number_type MAX>
bool subset_sum1(number_type sum, const vector<number_type>& v) {
    bitset<MAX + 1> b = 1;
    for (const auto& i : v) b |= b << i;
    return b.test(sum);
}

// 部分和問題
// O(n * A)
template<typename number_type>
bool subset_sum2(number_type sum, const vector<number_type>& v) {
    int n = v.size();
    // operator|= is unavailable for vector<bool> element references
    vector< vector<int> > dp(n + 1, vector<int>(sum + 1, 0));
    dp.at(0).at(0) = 1;
    for (int i = 0; i < n; ++i) for (number_type j = 0; j <= sum; ++j) {
        if (j + v.at(i) <= sum) dp.at(i + 1).at(j + v.at(i)) |= dp.at(i).at(j);
        dp.at(i + 1).at(j) |= dp.at(i).at(j);
    }
    return dp.at(n).at(sum);
}

// 部分和数え上げ問題
// O(n * A)
template<mod_type MOD, typename number_type>
Fp<MOD> count_subset_sum(number_type sum, const vector<number_type>& v) {
    vector< Fp<MOD> > dp(sum + 1);
    dp.at(0) = 1;
    for (const auto& i : v) for (number_type j = sum - i; j >= 0; --j) dp.at(j + i) += dp.at(j);
    return dp.at(sum);
}

// 最小個数部分和問題
// O(n * A)
template<typename number_type>
int minimum_subset_sum(number_type sum, const vector<number_type>& v, int impossible = -1) {
    constexpr int INF = (1 << 30);
    vector<int> dp(sum + 1, INF);
    dp.at(0) = 0;
    for (const auto& i : v) for (number_type j = sum - i; j >= 0; --j) upd_min(dp.at(j + i), dp.at(j) + 1);
    return dp.at(sum) == INF ? impossible : dp.at(sum);
}

// ・1≤K≤n≤500
// ・1≤A≤10000
// ・1≤a[i]≤1000

// K個以内部分和問題
// O(n * k * MAX(A))
template<typename number_type, number_type MAX>
bool capped_subset_sum1(number_type sum, const vector<number_type>& v, int maximum) {
    vector< bitset<MAX + 1> > b(maximum + 1);
    b.at(0).set(0);
    for (const auto& i : v) for (int j = maximum - 1; j >= 0; --j) b.at(j + 1) |= b.at(j) << i;
    return any_of(b.begin(), b.end(), [sum](auto i) {
        return i.test(sum);
    });
}

// K個以内部分和問題
// O(n * A)
template<typename number_type>
bool capped_subset_sum2(number_type sum, const vector<number_type>& v, int maximum) {
    return minimum_subset_sum(sum, v, maximum + 1) <= maximum;
}

// ・1≤n≤500
// ・1≤A≤10000
// ・1≤a[i],m[i]≤10000

// 個数制限付き部分和問題
// O(sum(m) * MAX(A))
template<typename number_type, number_type MAX>
bool subset_sum1(number_type sum, const vector< pair<number_type, int> >& v) {
    bitset<MAX + 1> b = 1;
    for (const auto& i : v) for (int j = 0; j < i.second; ++j) b |= b << i.first;
    return b.test(sum);
}

// O(n * A)
template<typename number_type, number_type MAX>
bool subset_sum2(number_type sum, const vector< pair<number_type, int> >& v) {
    int n = v.size();
    vector<int> dp(sum + 1, -1);
    dp.at(0) = 0;
    for (int i = 0; i < n; ++i) for (number_type j = 0; j <= sum; ++j) {
        if (dp.at(j) >= 0) dp.at(j) = v.at(i).second;
        else if (j >= v.at(i).first) upd_max(dp.at(j), dp.at(j - v.at(i).first) - 1);
    }
    return dp.at(sum) >= 0;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    constexpr int MOD = 1000000007;
    constexpr int MAX = 10000;
    int n, a, k;
    cin >> n >> a >> k;
    vector<int> v(n);
    for (auto&& i : v) cin >> i;
    cout << (subset_sum1<int, MAX>(a, v) ? "Yes" : "No") << '\n';
    cout << (subset_sum2(a, v) ? "Yes" : "No") << '\n';
    cout << count_subset_sum<MOD>(a, v) << '\n';
    cout << minimum_subset_sum(a, v) << '\n';
    cout << (capped_subset_sum1<int, MAX>(a, v, k) ? "Yes" : "No") << '\n';
    cout << (capped_subset_sum2(a, v, k) ? "Yes" : "No") << '\n';

    vector< pair<int, int> > u(n);
    for (auto&& i : u) cin >> i.first >> i.second;
    cout << (subset_sum1<int, MAX>(a, u) ? "Yes" : "No") << '\n';
    cout << (subset_sum2<int, MAX>(a, u) ? "Yes" : "No") << '\n';
    return 0;
}
