//
// Created by Marcel Hofmann on 22.09.23.
//

#include "../sprite/sprite.h"

uint32_t rgbToUint32(uint8_t r, uint8_t g, uint8_t b) {
    return (r << 24) | (g << 16) | (b << 8) | 255;
}

int allAliensDead (Game& game) {
    for (int i = 0; i < game.numberOfAliens; i++) {
        Alien &alien = game.aliens[i];
       if (alien.type != ALIEN_DEAD) {
           return false;
       }
    }

    return true;
}

int getAlienMaxX (Game& game)
{
    int maxX {0};

    for (int i = 0; i < game.numberOfAliens; i++) {
        Alien &alien = game.aliens[i];
        if (alien.x > maxX && alien.type != ALIEN_DEAD) {
            maxX = (int)alien.x;
        }
    }

    return maxX;
}

int getAlienMinX (Game& game)
{
    int minX {(int)game.width};

    for (int i = 0; i < game.numberOfAliens; i++) {
        Alien &alien = game.aliens[i];
        if (alien.x < minX && alien.type != ALIEN_DEAD) {
            minX = (int)alien.x;
        }
    }

    return minX;
}

Sprite *createAlienSprites() {
    static Sprite alienSprites[6];

    alienSprites[0].width = 8;
    alienSprites[0].height = 8;
    alienSprites[0].data = new uint8_t[64]
            {
                    0, 0, 0, 1, 1, 0, 0, 0, // ...@@...
                    0, 0, 1, 1, 1, 1, 0, 0, // ..@@@@..
                    0, 1, 1, 1, 1, 1, 1, 0, // .@@@@@@.
                    1, 1, 0, 1, 1, 0, 1, 1, // @@.@@.@@
                    1, 1, 1, 1, 1, 1, 1, 1, // @@@@@@@@
                    0, 1, 0, 1, 1, 0, 1, 0, // .@.@@.@.
                    1, 0, 0, 0, 0, 0, 0, 1, // @......@
                    0, 1, 0, 0, 0, 0, 1, 0  // .@....@.
            };

    alienSprites[1].width = 8;
    alienSprites[1].height = 8;
    alienSprites[1].data = new uint8_t[64]
            {
                    0, 0, 0, 1, 1, 0, 0, 0, // ...@@...
                    0, 0, 1, 1, 1, 1, 0, 0, // ..@@@@..
                    0, 1, 1, 1, 1, 1, 1, 0, // .@@@@@@.
                    1, 1, 0, 1, 1, 0, 1, 1, // @@.@@.@@
                    1, 1, 1, 1, 1, 1, 1, 1, // @@@@@@@@
                    0, 0, 1, 0, 0, 1, 0, 0, // ..@..@..
                    0, 1, 0, 1, 1, 0, 1, 0, // .@.@@.@.
                    1, 0, 1, 0, 0, 1, 0, 1  // @.@..@.@
            };

    alienSprites[2].width = 11;
    alienSprites[2].height = 8;
    alienSprites[2].data = new uint8_t[88]
            {
                    0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, // ..@.....@..
                    0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, // ...@...@...
                    0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, // ..@@@@@@@..
                    0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, // .@@.@@@.@@.
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // @@@@@@@@@@@
                    1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, // @.@@@@@@@.@
                    1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, // @.@.....@.@
                    0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0  // ...@@.@@...
            };

    alienSprites[3].width = 11;
    alienSprites[3].height = 8;
    alienSprites[3].data = new uint8_t[88]
            {
                    0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, // ..@.....@..
                    1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, // @..@...@..@
                    1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, // @.@@@@@@@.@
                    1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, // @@@.@@@.@@@
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // @@@@@@@@@@@
                    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, // .@@@@@@@@@.
                    0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, // ..@.....@..
                    0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0  // .@.......@.
            };

    alienSprites[4].width = 12;
    alienSprites[4].height = 8;
    alienSprites[4].data = new uint8_t[96]
            {
                    0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, // ....@@@@....
                    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, // .@@@@@@@@@@.
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // @@@@@@@@@@@@
                    1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, // @@@..@@..@@@
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // @@@@@@@@@@@@
                    0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, // ...@@..@@...
                    0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, // ..@@.@@.@@..
                    1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1  // @@........@@
            };


    alienSprites[5].width = 12;
    alienSprites[5].height = 8;
    alienSprites[5].data = new uint8_t[96]
            {
                    0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, // ....@@@@....
                    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, // .@@@@@@@@@@.
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // @@@@@@@@@@@@
                    1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, // @@@..@@..@@@
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // @@@@@@@@@@@@
                    0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, // ..@@@..@@@..
                    0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, // .@@..@@..@@.
                    0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0  // ..@@....@@..
            };

    return alienSprites;
}

Sprite createDeathAlien() {
    Sprite alienSprite{};
    alienSprite.width = 13;
    alienSprite.height = 7;
    alienSprite.data = new uint8_t[91]
            {
                    0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, // .@..@...@..@.
                    0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, // ..@..@.@..@..
                    0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, // ...@.....@...
                    1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, // @@.........@@
                    0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, // ...@.....@...
                    0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, // ..@..@.@..@..
                    0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0  // .@..@...@..@.
            };

    return alienSprite;
}

Sprite createPlayer() {
    Sprite player{};

    player.width = 11;
    player.height = 7;
    player.data = new uint8_t[77]
            {
                    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, // .....@.....
                    0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, // ....@@@....
                    0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, // ....@@@....
                    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, // .@@@@@@@@@.
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // @@@@@@@@@@@
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // @@@@@@@@@@@
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // @@@@@@@@@@@
            };

    return player;
}

Sprite createBullet() {
    Sprite bullet{};
    bullet.width = 1;
    bullet.height = 3;
    bullet.data = new uint8_t[3]
            {
                    1, 1, 1
            };

    return bullet;
}
