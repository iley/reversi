#ifndef GAMEFIELD_HPP
#define GAMEFIELD_HPP

#include <cassert>
#include <vector>
#include <SDL/SDL.h>
#include "matrix.hpp"

enum {
    CHIP_NONE = 0,
    CHIP_WHITE,
    CHIP_BLACK,
};

struct Point
{
    int row, col;

    Point(int r, int c) : row(r), col(c) {}
};

class GameField
{
public:
    GameField(int rows, int cols);
    GameField(const GameField &other) : matrix(other.matrix) {}

    void Reset();

    int Rows() const { return matrix.Rows(); }
    int Cols() const { return matrix.Cols(); }

    void Draw(SDL_Surface *screen) const;

    int Get(int row, int col) const;

    bool Move(int color, int row, int col);
    bool HasMoves(int color) const;

    std::vector<Point> CorrectMoves(int color) const;

    int Score(int color) const;

private:
    Matrix<int> matrix;
};

int OpponentColor(int color);

#endif
