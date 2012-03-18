#include "gamefield.hpp"
#include "reversi.hpp"
#include <SDL/SDL_gfxPrimitives.h>
#include <cstdio>
#include <cstring>

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

IntelibX_not_a_point::IntelibX_not_a_point(SReference a_param)
    : IntelibX("Not a point", a_param) {}

IntelibTypeId PointExpression::TypeId(&SExpression::TypeId, true);

SString PointExpression::TextRepresentation() const
{
    static char buffer[32];
    sprintf(buffer, "(%d, %d)", row, col);
    return buffer;
}

IntelibX_not_a_game_field::IntelibX_not_a_game_field(SReference a_param)
    : IntelibX("Not a game field", a_param) {}

IntelibTypeId GameFieldExpression::TypeId(&SExpression::TypeId, true);

SExpression *GameFieldExpression::Clone() const {
    return new GameFieldExpression(*this);
}

void GameFieldExpression::Reset()
{
    for (int i = 0; i < Rows(); ++i)
        for (int j = 0; j < Cols(); ++j)
            matrix(i,j) = CHIP_NONE;

    int r = Rows()/2, c = Cols()/2;
    matrix(r,c) = matrix(r-1,c-1) = CHIP_WHITE;
    matrix(r-1,c) = matrix(r,c-1) = CHIP_BLACK;
}

void GameFieldExpression::Draw(SDL_Surface *screen) const
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

int GameFieldExpression::Get(int row, int col) const
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

bool GameFieldExpression::Move(int color, int row, int col)
{
    using namespace std;

    if (matrix(row, col) != CHIP_NONE)
        return false;

    int chipsFlipped = 0;

    for (int dir = 0; dir < directions; ++dir) {
        int i = row, j = col;
        SReference toFlip = *PTheEmptyList;

        while (true) {
            i += shift[dir][0];
            j += shift[dir][1];

            if (matrix.CheckBounds(i,j) && matrix(i,j) == OpponentColor(color))
                toFlip = Point(i,j) ^ toFlip;
            else
                break;
        }

        if (matrix.CheckBounds(i,j) && matrix(i,j) == color) {
            for (SReference p = toFlip; !p.IsEmptyList(); p = p.Cdr()) {
                Point point = p.Car();
                matrix(point->Row(), point->Col()) = color;
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

bool GameFieldExpression::HasMoves(int color) const
{
    GameField tmp(Clone());

    for (int i = 0; i < Rows(); ++i)
        for (int j = 0; j < Cols(); ++j)
            if (tmp->Move(color, i, j))
                return true;

    return false;
}

SReference GameFieldExpression::CorrectMoves(int color) const
{
    SReference result = *PTheEmptyList;

    for (int i = 0; i < Rows(); ++i)
        for (int j = 0; j < Cols(); ++j) {
            GameField tmp(Clone());
            if (tmp->Move(color, i, j)) {
                SReference cell = Point(i,j) ^ tmp;
                result = cell ^ result;
            }
        }

    return result;
}

int GameFieldExpression::Score(int color) const
{
    int result = 0;

    for (int i = 0; i < Rows(); ++i)
        for (int j = 0; j < Cols(); ++j)
            if (matrix(i,j) == color)
                ++result;

    return result;
}

SString GameFieldExpression::TextRepresentation() const
{
    return "<GAME FIELD>";
}
