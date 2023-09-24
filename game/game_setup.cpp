//
// Created by Marcel Hofmann on 23.09.23.
//

#include "game.h"

void prepareGame(Game &game, Sprite *&alienSprites, Sprite &deathSprite, SpriteAnimation *alienAnimation, uint8_t *&deathCounters, Buffer &buffer, int &alienUpdateFrequency) {
    for (size_t i = 0; i < 3; ++i) {
        alienAnimation[i].loop = true;
        alienAnimation[i].numberOfFrames = 2;
        alienAnimation[i].frameDuration = alienUpdateFrequency;
        alienAnimation[i].time = 0;

        alienAnimation[i].frames = new Sprite *[2];
        alienAnimation[i].frames[0] = &alienSprites[2 * i];
        alienAnimation[i].frames[1] = &alienSprites[2 * i + 1];
    }

    game.width = buffer.width;
    game.height = buffer.height;

    game.aliens = new Alien[game.numberOfAliens];

    game.player.x = 112 - 5;
    game.player.y = 32;
    game.player.life = 3;

    // Init aliens
    for (size_t yi = 0; yi < 5; ++yi) {
        for (size_t xi = 0; xi < 11; ++xi) {
            Alien &alien = game.aliens[yi * 11 + xi];
            alien.type = (5 - yi) / 2 + 1;

            const Sprite &sprite = alienSprites[2 * (alien.type - 1)];

            alien.x = 16 * xi + 20 + (deathSprite.width - sprite.width) / 2;
            alien.y = 17 * yi + 128;
        }
    }

    for (size_t i = 0; i < game.numberOfAliens; i++) {
        deathCounters[i] = 10;
    }
}
