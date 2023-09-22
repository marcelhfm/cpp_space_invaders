//
// Created by Marcel Hofmann on 22.09.23.
//

#ifndef CPP_SPACE_INVADERS_GAME_H
#define CPP_SPACE_INVADERS_GAME_H

#include <cstdio>
#include "../structs/structs.h"

struct Game {
    size_t width, height;
    size_t num_aliens;

    Alien* aliens;
    Player player;

};

#endif //CPP_SPACE_INVADERS_GAME_H
