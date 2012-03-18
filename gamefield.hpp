#ifndef GAMEFIELD_HPP
#define GAMEFIELD_HPP

#include <cassert>
#include <vector>
#include <SDL/SDL.h>
#include <sexpress/sexpress.hpp>
#include <sexpress/gensref.hpp>
#include <sexpress/sstring.hpp>
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

class IntelibX_not_a_game_field : public IntelibX
{
public:
    IntelibX_not_a_game_field(SReference a_param);
};

class GameFieldExpression : public SExpression
{
    friend class GameField;
public:
    static IntelibTypeId TypeId;

    virtual SExpression *Clone() const;

    void Reset();

    int Rows() const
        { return matrix.Rows(); }

    int Cols() const
        { return matrix.Cols(); }

    void Draw(SDL_Surface *screen) const;

    int Get(int row, int col) const;

    bool Move(int color, int row, int col);
    bool HasMoves(int color) const;

    std::vector<Point> CorrectMoves(int color) const;

    int Score(int color) const;

    virtual SString TextRepresentation() const;

private:
    Matrix<int> matrix;

    GameFieldExpression(int rows, int cols) : SExpression(TypeId), matrix(rows, cols)
        { Reset(); }

    GameFieldExpression(const GameFieldExpression &other) : SExpression(TypeId), matrix(other.matrix)
        {}
};

typedef GenericSReference<GameFieldExpression, IntelibX_not_a_game_field> GameField_Super;

class GameField : public GameField_Super
{
public:
    GameField(int rows, int cols) : GameField_Super(new GameFieldExpression(rows, cols))
        {}

    GameField(const SReference &ref) : GameField_Super(ref)
        {}
};

int OpponentColor(int color);

#endif
