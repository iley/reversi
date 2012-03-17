#include "gamefield.hpp"
#include "reversi.hpp"
#include <SDL/SDL_gfxPrimitives.h>
#include <stdio.h>
#include <algorithm>
#include <vector>

unsigned int color(unsigned int r, unsigned int g, unsigned int b, unsigned int a = 0xff)
{
    return (r << 24) | (g << 16) | (b << 8) | a;
}

using std::min;

int OpponentColor(int color)
{
    if (color == CHIP_WHITE) {
        return CHIP_BLACK;
    } else {
        assert(color == CHIP_BLACK);
        return CHIP_WHITE;
    }
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

    int r = Rows()/2, c = Cols()/2;
    matrix(r,c) = matrix(r-1,c-1) = CHIP_WHITE;
    matrix(r-1,c) = matrix(r,c-1) = CHIP_BLACK;
}

void GameField::Draw(SDL_Surface *screen) const
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

int GameField::Get(int row, int col) const
{
    return matrix(row, col);
}

static const int directions = 8;
static int shift[directions][2] = {
    {  1,  0 },
    { -1,  0 },
    {  0,  1 },
    {  0, -1 },
    {  1,  1 },
    { -1,  1 },
    {  1, -1 },
    { -1, -1 },
};

bool GameField::Move(int color, int row, int col)
{
    using namespace std;

    if (matrix(row, col) != CHIP_NONE)
        return false;

    int chipsFlipped = 0;

    for (int dir = 0; dir < directions; ++dir) {
        int i = row, j = col;
        vector<Point> toFlip;

        while (true) {
            i += shift[dir][0];
            j += shift[dir][1];

            if (matrix.CheckBounds(i,j) && matrix(i,j) == OpponentColor(color))
                toFlip.push_back(Point(i,j));
            else
                break;
        }

        if (matrix.CheckBounds(i,j) && matrix(i,j) == color) {
            for (auto it = toFlip.begin(); it != toFlip.end(); ++it) {
                matrix(it->row, it->col) = color;
                ++chipsFlipped;
            }
        }
    }

    if (chipsFlipped > 0) {
        matrix(row, col) = color;
        return true;
    } else {
        return false;
    }
}

bool GameField::HasMoves(int color) const
{
    GameField tmp(*this);

    for (int i = 0; i < Rows(); ++i)
        for (int j = 0; j < Cols(); ++j)
            if (tmp.Move(color, i, j))
                return true;

    return false;
}

std::vector<Point> GameField::CorrectMoves(int color) const
{
    std::vector<Point> result;

    for (int i = 0; i < Rows(); ++i)
        for (int j = 0; j < Cols(); ++j) {
            GameField tmp(*this);
            if (tmp.Move(color, i, j))
                result.push_back(Point(i,j));
        }

    return result;
}

int GameField::Score(int color) const
{
    int result = 0;

    for (int i = 0; i < Rows(); ++i)
        for (int j = 0; j < Cols(); ++j)
            if (matrix(i,j) == color)
                ++result;

    return result;
}
