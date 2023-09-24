//
// Created by Marcel Hofmann on 22.09.23.
//

#ifndef CPP_SPACE_INVADERS_GAME_H
#define CPP_SPACE_INVADERS_GAME_H

#include <cstdio>
#include <cstdint>

struct Sprite
{
    size_t width, height;
    uint8_t* data;
};

struct SpriteAnimation {
    __attribute__((unused)) bool loop;
    size_t numberOfFrames;
    size_t frameDuration;
    size_t time;
    Sprite **frames;
};

const int GAME_MAX_BULLETS = 128;

Sprite createDeathAlien();
Sprite *createAlienSprites();
Sprite createPlayer();
Sprite createBullet();
uint32_t rgbToUint32(uint8_t r, uint8_t g, uint8_t b);


struct Buffer {
    size_t width, height;
    uint32_t *data;
};

struct Alien {
    size_t x, y;
    uint8_t type;
};

struct Player {
    size_t x, y;
    size_t life;
};

struct Bullet {
    size_t x, y;
    int dir;
};

struct Game {
    size_t width, height;
    size_t numberOfAliens;
    size_t numberOfBullets;

    Alien* aliens;
    Player player;
    Bullet bullets[GAME_MAX_BULLETS];
};

enum AlienType: uint8_t
{
    ALIEN_DEAD = 0,
    ALIEN_TYPE_A = 1,
    ALIEN_TYPE_B = 2,
    ALIEN_TYPE_C = 3
};

int getAlienMaxX (Game& game);
int getAlienMinX (Game& game);
int allAliensDead (Game& game);

#endif //CPP_SPACE_INVADERS_GAME_H
