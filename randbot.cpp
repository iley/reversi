#include "randbot.hpp"
#include <cstdlib>

void RandBot::Move(GameField &field, MoveCallback callback)
{
    std::vector<Point> moves = field.CorrectMoves(Color());

    int x = moves.size() * rand() / RAND_MAX;
    callback(moves[x].row, moves[x].col);
}
