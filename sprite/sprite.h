//
// Created by Marcel Hofmann on 22.09.23.
//



#ifndef CPP_SPACE_INVADERS_SPRITE_H
#define CPP_SPACE_INVADERS_SPRITE_H

#include "../game/game.h"
#include <cstdio>
#include <cstdint>


struct SpriteAnimation {
    bool loop;
    size_t numberOfFrames;
    size_t frameDuration;
    size_t time;
    Sprite **frames;
};

bool checkSpriteOverlap(
        const Sprite &sp_a, size_t x_a, size_t y_a,
        const Sprite &sp_b, size_t x_b, size_t y_b
);

void drawSpriteBuffer(Buffer *buffer, const Sprite &sprite, size_t x, size_t y, uint32_t color);

#endif //CPP_SPACE_INVADERS_SPRITE_H
