#include "randbot.hpp"
#include "reversi.hpp"
#include "gamefield.hpp"
#include <cstdlib>

void RandBot::Move(const GameField &field, MoveCallback callback)
{
    SReference moves = field->CorrectMoves(Color());

    int count = 0;
    for (SReference p = moves; !p.IsEmptyList(); p = p.Cdr())
        ++count;

    int x = count * rand() / RAND_MAX;

    SReference p = moves;
    for (int i = 0; i < x; ++i)
        p = p.Cdr();

    Point point = p.Car().Car();
    callback(point->Row(), point->Col());
}
