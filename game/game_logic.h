//
// Created by Marcel Hofmann on 23.09.23.
//

#ifndef CPP_SPACE_INVADERS_GAME_LOGIC_H
#define CPP_SPACE_INVADERS_GAME_LOGIC_H

#include "game.h"

void simulateAliens(Game &game, __attribute__((unused)) SpriteAnimation *alienAnimation, uint8_t *deathCounters);

void simulateBullets(Game &game, Sprite &bulletSprite, Sprite &deathSprite, SpriteAnimation *alienAnimation,
                     uint32_t &score);

void simulatePlayer(Game &game, Sprite &playerSprite, int &playerMoveDirection);

void processEvents(Game &game, Sprite &playerSprite);

void drawScene(Buffer &buffer, Game &game, SpriteAnimation *alienAnimation, const uint8_t *deathCounters,
               Sprite &playerSprite,
               Sprite &bulletSprite, Sprite &deathSprite, Sprite &textSpriteSheet, Sprite &numberSpriteSheet,
               uint32_t &score);

#endif //CPP_SPACE_INVADERS_GAME_LOGIC_H
