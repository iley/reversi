#ifndef PLGBOT_HPP
#define PLGBOT_HPP

#include "player.hpp"

class PlgBot : public Player
{
public:
    PlgBot(int color) : Player(color) {}

    virtual void Move(GameField &field, MoveCallback callback);
};

#endif
