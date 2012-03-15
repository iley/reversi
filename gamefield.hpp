#ifndef GAMEFIELD_HPP
#define GAMEFIELD_HPP

#include "matrix.hpp"
#include "SDL/SDL.h"

enum {
    CHIP_NONE = 0,
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
    void Draw(SDL_Surface *screen);

private:
    Matrix<int> matrix;
};

#endif
