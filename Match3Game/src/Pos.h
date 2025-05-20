#pragma once

struct Pos {
    int row, col;
    auto operator<=>(Pos const&) const = default;
    Pos() : row(0), col(0) {}
    Pos(int row_, int col_) : row(row_), col(col_){}
};