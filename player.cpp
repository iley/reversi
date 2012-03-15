#include "player.hpp"
#include <vector>
#include <cassert>

using std::vector;

Player::~Player() {}

vector<HumanPlayer*> HumanPlayer::players;

void HumanPlayer::HandleInput(int row, int col)
{
    for (vector<HumanPlayer*>::iterator it = players.begin(); it != players.end(); ++it) {
        if ((*it)->FinishMove(row, col))
            break;
    }
}

HumanPlayer::HumanPlayer(int color) : Player(color), waiting(false), callback(0) {
    players.push_back(this);
}

bool HumanPlayer::FinishMove(int row, int col)
{
    if (waiting) {
        assert(callback);
        if (callback(row, col)) {
            waiting = false;
            callback = 0;
        }
        return true;
    } else {
        return false;
    }
}

void HumanPlayer::Move(GameField &field, MoveCallback cb)
{
    callback = cb;
    waiting = true;
}
