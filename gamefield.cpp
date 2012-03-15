#include "gamefield.hpp"
#include "reversi.hpp"
#include <SDL/SDL_gfxPrimitives.h>
#include <stdio.h>

unsigned int color(unsigned int r, unsigned int g, unsigned int b, unsigned int a = 0xff)
{
    return (r << 24) | (g << 16) | (b << 8) | a;
}

template <class Tp> Tp min(Tp x, Tp y)
{
    return x < y ? x : y;
}

const unsigned int
    gridColor = color(0x00, 0x00, 0x00),
    chipBorderColor = color(0x00, 0x00, 0x00),
    blackChipColor = color(0x00, 0x00, 0x00),
    whiteChipColor = color(0xff, 0xff, 0xff);

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
    
    int w = (ScreenWidth / Cols()) / 2;
    int h = (ScreenHeight / Rows()) / 2;
    int r = min(w, h) - 5;

    for (int i = 0; i < Rows(); ++i)
        for (int j = 0; j < Cols(); ++j) {
            if (matrix(i,j) == CHIP_NONE)
                continue;

            int color;
            if (matrix(i,j) == CHIP_WHITE)
                color = whiteChipColor;
            else
                color = blackChipColor;

            filledCircleColor(screen, i * ScreenHeight / Rows() + h, j * ScreenWidth / Cols() + w, r, color);
        }
}
