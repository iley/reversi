#ifndef GAMEFIELD_HPP
#define GAMEFIELD_HPP

#include "matrix.hpp"

enum {
    CHIP_NONE,
    CHIP_WHITE,
    CHIP_BLACK,
};

class GameField
{
public:
    GameField(int rows, int cols);

    void Reset();
    int Rows() const { return matrix.Rows(); }
    int Cols() const { return matrix.Cols(); }

private:
    Matrix<int> matrix;
};

#endif
