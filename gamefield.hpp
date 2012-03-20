#ifndef GAMEFIELD_HPP
#define GAMEFIELD_HPP

#include <cassert>
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

int OpponentColor(int color);

class IntelibX_not_a_point : public IntelibX
{
public:
    IntelibX_not_a_point(SReference a_param);
};

class PointExpression : public SExpression
{
    friend class Point;
public:
    static IntelibTypeId TypeId;

    int Row() const
        { return row; }

    int Col() const
        { return col; }

    virtual SString TextRepresentation() const;

private:
    int row, col;

    PointExpression(int r, int c) : SExpression(TypeId), row(r), col(c)
        {}

    PointExpression(const PointExpression &other) : SExpression(TypeId), row(other.row), col(other.col)
        {}
};

typedef GenericSReference<PointExpression, IntelibX_not_a_point> Point_Super;

class Point : public Point_Super
{
public:
    Point(int row, int col) : Point_Super(new PointExpression(row, col))
        {}

    Point(SReference &ref) : Point_Super(ref) {}
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

     SReference CorrectMoves(int color) const;

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
