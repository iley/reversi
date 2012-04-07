#include "player.hpp"
#include "reversi.hpp"
#include <vector>
#include <cassert>

using std::vector;

Player::~Player() {}

vector<HumanPlayer*> HumanPlayer::players;

void HumanPlayer::HandleInput(int row, int col)
{
    for (auto it = players.begin(); it != players.end(); ++it) {
        if ((*it)->FinishMove(row, col))
            break;
    }
}

HumanPlayer::HumanPlayer(int color) : Player(color), waiting(false), callback(0) {
    players.push_back(this);
}

bool HumanPlayer::FinishMove(int row, int col)
{
    debug("finish move");

    if (waiting) {
        assert(callback);
        waiting = false;
        if (!callback(row, col)) {
            waiting = true;
        }
        return true;
    } else {
        debug("not waiting");

        return false;
    }
}

void HumanPlayer::Move(const GameField &field, MoveCallback cb)
{
    debug("waiting for human move");

    callback = cb;
    waiting = true;
}
