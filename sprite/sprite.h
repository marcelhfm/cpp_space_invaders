//
// Created by Marcel Hofmann on 22.09.23.
//

#ifndef CPP_SPACE_INVADERS_SPRITE_H
#define CPP_SPACE_INVADERS_SPRITE_H

#include <cstdio>
#include <cstdint>
#include "../game/game.h"



bool checkSpriteOverlap(
        const Sprite &sp_a, size_t x_a, size_t y_a,
        const Sprite &sp_b, size_t x_b, size_t y_b
);

Sprite createNumberSpriteSheet();
Sprite createTextSpriteSheet();
void drawTextBuffer(Buffer *buffer, const Sprite& textSpriteSheet, const char* text, size_t x, size_t y, uint32_t color);
void drawNumberBuffer(Buffer* buffer, const Sprite& numberSpriteSheet, size_t number, size_t x, size_t y, uint32_t color);
void drawSpriteBuffer(Buffer *buffer, const Sprite &sprite, size_t x, size_t y, uint32_t color);

#endif //CPP_SPACE_INVADERS_SPRITE_H
