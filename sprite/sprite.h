//
// Created by Marcel Hofmann on 22.09.23.
//

#ifndef CPP_SPACE_INVADERS_SPRITE_H
#define CPP_SPACE_INVADERS_SPRITE_H


#include "../structs/structs.h"
#include <cstdio>
#include <cstdint>

struct Sprite
{
    size_t width, height;
    uint8_t* data;
};

struct SpriteAnimation
{
    bool loop;
    size_t numberOfFrames;
    size_t frameDuration;
    size_t time;
    Sprite** frames;
};

SpriteAnimation createAlienSpriteAnimation(Sprite& alienSprite0, Sprite& alienSprite1 );
Sprite createAlien1();
Sprite createAlien0();
void drawSpriteBuffer(Buffer* buffer, const Sprite& sprite, size_t x, size_t y, uint32_t color);

#endif //CPP_SPACE_INVADERS_SPRITE_H
