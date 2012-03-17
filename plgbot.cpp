#include "plgbot.hpp"

static bool predicateMoves(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
{
    return true;
}

PlgBot::PlgBot(int color) : Player(color)
{
}

void PlgBot::Move(GameField &field, MoveCallback callback)
{
}
