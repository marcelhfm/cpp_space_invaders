//
// Created by Marcel Hofmann on 23.09.23.
//

#ifndef CPP_SPACE_INVADERS_GAME_SETUP_H
#define CPP_SPACE_INVADERS_GAME_SETUP_H

#include "../sprite/sprite.h"
#include <cstdint>

void prepareGame(Game &game, Sprite *&alienSprites, Sprite &deathSprite, SpriteAnimation *alienAnimation, uint8_t *&deathCounters, Buffer &buffer);

#endif //CPP_SPACE_INVADERS_GAME_SETUP_H
