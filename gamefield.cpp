#include "gamefield.hpp"

GameField::GameField(int rows, int cols) : matrix(rows, cols)
{
    Reset();
}

void GameField::Reset()
{
    for (int i = 0; i < Rows(); ++i)
        for (int j = 0; j < Cols(); ++j)
            matrix(i,j) = CHIP_NONE;
}
