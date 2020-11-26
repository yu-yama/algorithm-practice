// dfs/sudoku.cpp
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
#include <bitset>
#include <optional>
#include <array>
#include <string>
#include <algorithm>

template<int B = 3>
struct Sudoku {
    constexpr static int N = B * B, EMPTY = 0;
    constexpr static char EMPTYC = '_';
    static_assert(B > 0, "block size must be positive");
    static_assert(EMPTY == 0 || EMPTY > N, "default value for empty cells must be positive and must not overlap with valid values");
    using field_array_type = std::array< std::array<int, N>, N>;
    static std::string output_space, output_line_break;
    static bool integral_input;
    static int input_empty_int;
    static char input_empty_char;
private:
    struct SudokuField : field_array_type {
        SudokuField() noexcept : field_array_type() {}
        SudokuField(field_array_type f, int empty_num = EMPTY) noexcept : field_array_type(f) {
            if (empty_num != EMPTY) for (int i = 0; i < N; ++i) for (int j = 0; j < N; ++j) if ((*this)[i][j] == empty_num) (*this)[i][j] = EMPTY;
        }
        SudokuField(const std::vector< std::vector<int> >& v, int empty_num = EMPTY) : SudokuField() {
            for (int i = 0; i < N; ++i) for (int j = 0; j < N; ++j) (*this)[i][j] = (v[i][j] == empty_num ? EMPTY : v[i][j]);
        }
        SudokuField(const std::vector< std::vector<char> >& v, char empty_char = EMPTYC) : SudokuField() {
            for (int i = 0; i < N; ++i) for (int j = 0; j < N; ++j) (*this)[i][j] = (v[i][j] == empty_char ? EMPTY : v[i][j] - '0');
        }
        SudokuField(const std::vector<std::string>& v, char empty_char = EMPTYC) : SudokuField() {
            for (int i = 0; i < N; ++i) for (int j = 0; j < N; ++j) (*this)[i][j] = (v[i][j] == empty_char ? EMPTY : v[i][j] - '0');
        }
        friend std::istream& operator>>(std::istream& s, SudokuField& a) {
            if (integral_input) {
                field_array_type t;
                for (auto&& i : t) for (auto&& j : i) s >> j;
                a = SudokuField(std::move(t), input_empty_int);
            } else {
                std::vector<std::string> t(N);
                for (auto&& i : t) s >> i;
                a = SudokuField(t, input_empty_char);
            }
            return s;
        }
        friend std::ostream& operator<<(std::ostream& s, const SudokuField& a) {
            for (const auto& i : a) {
                for (const auto& j : i) s << j << output_space;
                s << output_line_break;
            }
            return s;
        }
    };
    using field_type = SudokuField;
    field_type field_;
    bool solved;
    std::vector<field_type> solutions_;
    constexpr std::optional< std::pair<int, int> > find_empty() const noexcept {
        for (int i = 0; i < N; ++i) for (int j = 0; j < N; ++j) if (field_[i][j] == EMPTY) return {{i, j}};
        return {};
    }
    void solve() {
        auto empty_cell = find_empty();
        if (!empty_cell.has_value()) {
            solutions_.push_back(field_);
            return;
        }
        auto [emptyy, emptyx] = empty_cell.value();

        std::bitset<N + 1> unavailable = 0;
        for (int i = 0; i < N; ++i) unavailable.set(field_[emptyy][i]).set(field_[i][emptyx]);
        for (int blocky = emptyy / B * B, blockx = emptyx / B * B, y = blocky; y < blocky + B; ++y) for (int x = blockx; x < blockx + B; ++x) unavailable.set(field_[y][x]);

        for (int v = 1; v <= N; ++v) {
            if (unavailable.test(v)) continue;
            field_[emptyy][emptyx] = v;
            solve();
        }

        field_[emptyy][emptyx] = EMPTY;
    }
public:
    template<typename... Args>
    Sudoku(Args... args) : field_(std::forward<Args>(args)...), solved(false), solutions_() {}
    const std::vector<field_type>& solutions() {
        if (!solved) solve();
        solved = true;
        return solutions_;
    }
    const field_type& field() const {
        return field_;
    }
    friend std::istream& operator>>(std::istream& s, Sudoku& a) {
        s >> a.field_;
        a.solved = false;
        a.solutions_.clear();
        return s;
    }
    friend std::ostream& operator<<(std::ostream& s, const Sudoku& a) {
        s << a.field_;
        return s;
    }
};
template<int B>
std::string Sudoku<B>::output_space = " ";
template<int B>
std::string Sudoku<B>::output_line_break = "\n";
template<int B>
bool Sudoku<B>::integral_input = false;
template<int B>
int Sudoku<B>::input_empty_int = Sudoku::EMPTY;
template<int B>
char Sudoku<B>::input_empty_char = Sudoku::EMPTYC;

template<int B = 3>
void test() {
    using namespace std;
    Sudoku<B> f;
    cout << "Empty:\n";
    cout << f << '\n';

    cin >> f;
    cout << "Input:\n";
    cout << f << '\n';

    cout << "Result:\n";
    if (f.solutions().size() == 0) cout << "Invalid puzzle: no solutions.\n";
    else if (f.solutions().size() > 1) cout << "Invalid puzzle: multiple solutions.\n";
    else cout << f.solutions()[0] << '\n';
}

int main() {
    using namespace std;
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    test<3>();
    test<2>();
    Sudoku<4>::integral_input = true;
    test<4>();
}
