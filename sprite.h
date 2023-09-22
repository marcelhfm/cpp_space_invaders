//
// Created by Marcel Hofmann on 22.09.23.
//

#ifndef CPP_SPACE_INVADERS_SPRITE_H
#define CPP_SPACE_INVADERS_SPRITE_H

#include <cstdio>
#include <cstdint>
#include "buffer.h"

struct Sprite
{
    size_t width, height;
    uint8_t* data;
};

void drawSpriteBuffer(Buffer* buffer, const Sprite& sprite, size_t x, size_t y, uint32_t color);

#endif //CPP_SPACE_INVADERS_SPRITE_H
