#include "player.h"

Player::Player()
{
    id = rand() % 100;
}

int Player::getId()
{
    return id;
}
