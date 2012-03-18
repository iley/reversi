#ifndef RANDBOT_HPP
#define RANDBOT_HPP

#include "player.hpp"
#include "gamefield.hpp"

class RandBot : public Player
{
public:
    RandBot(int color) : Player(color) {}

    virtual void Move(const GameField &field, MoveCallback callback);
};

#endif
