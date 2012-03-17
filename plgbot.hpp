#ifndef PLGBOT_HPP
#define PLGBOT_HPP

#include "player.hpp"
#include "bot.hpp"

class PlgBot : public Player
{
public:
    PlgBot(int color);

    virtual void Move(GameField &field, MoveCallback callback);
};

#endif
