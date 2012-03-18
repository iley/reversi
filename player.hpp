#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include "gamefield.hpp"

typedef bool (*MoveCallback)(int row, int col);

class Player
{
public:
    Player(int c) : color(c) {}
    virtual ~Player();

    int Color() const { return color; }
    virtual void Move(const GameField &field, MoveCallback callback) = 0;

private:
    int color;
};


class HumanPlayer : public Player
{
public:
    static void HandleInput(int row, int col);

    HumanPlayer(int color);

    bool FinishMove(int row, int col);
    virtual void Move(const GameField &field, MoveCallback cb);

private:
    static std::vector<HumanPlayer*> players;

    bool waiting;
    MoveCallback callback;
};

#endif
