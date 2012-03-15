#include "gamefield.hpp"
#include "reversi.hpp"
#include <SDL/SDL_gfxPrimitives.h>

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

void GameField::Draw(SDL_Surface *screen)
{

}
