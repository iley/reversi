#include "gamefield.hpp"
#include "reversi.hpp"
#include <SDL/SDL_gfxPrimitives.h>
#include <stdio.h>

unsigned int color(unsigned int r, unsigned int g, unsigned int b, unsigned int a = 0xff)
{
    return (r << 24) | (g << 16) | (b << 8) | a;
}

const unsigned int gridColor = color(0,0,0);

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

    for (int i = 1; i < Cols(); ++i)
        vlineColor(screen, i * ScreenWidth / Cols(), 0, ScreenHeight-1, gridColor);

    for (int i = 1; i < Rows(); ++i)
        hlineColor(screen, 0, ScreenWidth-1, i * ScreenHeight / Rows(), gridColor);
}
