//
// Created by Marcel Hofmann on 23.09.23.
//

#include "game_logic.h"
#include "../sprite/sprite.h"

extern bool firePressed;
extern int moveDir;

void
drawScene(Buffer &buffer, Game &game, SpriteAnimation *alienAnimation, const uint8_t *deathCounters,
          Sprite &playerSprite,
          Sprite &bulletSprite, Sprite &deathSprite, Sprite &textSpriteSheet, Sprite &numberSpriteSheet,
          uint32_t &score) {
    // Draw Score and bottom UI
    drawTextBuffer(&buffer, textSpriteSheet, "SCORE", 4, game.height - textSpriteSheet.height - 7,
                   rgbToUint32(128, 0, 0));
    drawNumberBuffer(&buffer, numberSpriteSheet, score, 4 + 2 * numberSpriteSheet.width,
                     game.height - 2 * numberSpriteSheet.height - 12,
                     rgbToUint32(128, 0, 0));
    drawTextBuffer(&buffer, textSpriteSheet, "CREDIT 00", 164, 7, rgbToUint32(128, 0, 0));
    for (size_t i = 0; i < game.width; ++i) {
        buffer.data[game.width * 16 + i] = rgbToUint32(128, 0, 0);
    }

    // Draw game
    for (size_t ai = 0; ai < game.numberOfAliens; ++ai) {
        if (!deathCounters[ai]) continue;

        const Alien &alien = game.aliens[ai];

        if (alien.type == ALIEN_DEAD) {
            drawSpriteBuffer(&buffer, deathSprite, alien.x, alien.y, rgbToUint32(128, 0, 0));
        } else {
            const SpriteAnimation &animation = alienAnimation[alien.type - 1];
            size_t currentFrame = animation.time / animation.frameDuration;
            const Sprite &sprite = *animation.frames[currentFrame];
            drawSpriteBuffer(&buffer, sprite, alien.x, alien.y, rgbToUint32(128, 0, 0));
        }
    }

    for (size_t bi = 0; bi < game.numberOfBullets; ++bi) {
        const Bullet &bullet = game.bullets[bi];
        const Sprite &sprite = bulletSprite;
        drawSpriteBuffer(&buffer, sprite, bullet.x, bullet.y, rgbToUint32(128, 0, 0));
    }

    drawSpriteBuffer(&buffer, playerSprite, game.player.x, game.player.y, rgbToUint32(128, 0, 0));

    //Update animations
    for (size_t i = 0; i < 3; ++i) {
        ++alienAnimation[i].time;
        if (alienAnimation[i].time == alienAnimation[i].numberOfFrames * alienAnimation[i].frameDuration) {
            alienAnimation[i].time = 0;
        }
    }
}


void simulateAliens(Game &game, __attribute__((unused)) SpriteAnimation *alienAnimation, uint8_t *deathCounters,
                    int& alienMoveDirection) {
    int maxX {getAlienMaxX(game)};
    int minX {getAlienMinX(game)};
    bool aliensChangedDirection {false};

    if (maxX >= game.width) {
        alienMoveDirection = -1;
        aliensChangedDirection = true;
    } else if (minX <= 0) {
        alienMoveDirection = 1;
        aliensChangedDirection = true;
    }

    for (size_t ai = 0; ai < game.numberOfAliens; ++ai) {
        Alien &alien = game.aliens[ai];
        if (alien.type == ALIEN_DEAD && deathCounters[ai]) {
            --deathCounters[ai];
        }

        if (alien.x + alienMoveDirection > game.width) {
            alien.x = game.width;
        } else if (alien.x - alienMoveDirection < 0) {
            alien.x = 0;
        } else {
            alien.x += alienMoveDirection;
        }

        if (aliensChangedDirection)
        {
            --alien.y;
        }
    }
}

void simulateBullets(Game &game, Sprite &bulletSprite, Sprite &deathSprite, SpriteAnimation *alienAnimation,
                     uint32_t &score) {
    for (size_t bi = 0; bi < game.numberOfBullets;) {
        game.bullets[bi].y += game.bullets[bi].dir;
        if (game.bullets[bi].y >= game.height || game.bullets[bi].y < bulletSprite.height) {
            game.bullets[bi] = game.bullets[game.numberOfBullets - 1];
            --game.numberOfBullets;
            continue;
        }

        // Check for a hit
        for (size_t ai = 0; ai < game.numberOfAliens; ai++) {
            const Alien &alien = game.aliens[ai];
            if (alien.type == ALIEN_DEAD) continue;

            const SpriteAnimation &animation = alienAnimation[alien.type - 1];
            size_t currentFrame = animation.time / animation.frameDuration;
            const Sprite &alienSprite = *animation.frames[currentFrame];
            bool overlap = checkSpriteOverlap(bulletSprite, game.bullets[bi].x, game.bullets[bi].y, alienSprite,
                                              alien.x, alien.y);
            if (overlap) {
                score += 10 * (4 - game.aliens[ai].type);
                game.aliens[ai].type = ALIEN_DEAD;
                game.aliens[ai].x -= (deathSprite.width - alienSprite.width) / 2;
                game.bullets[bi] = game.bullets[game.numberOfBullets - 1];
                --game.numberOfBullets;
                continue;
            }
        }

        ++bi;
    }
}

void simulatePlayer(Game &game, Sprite &playerSprite, int &playerMoveDirection) {
    playerMoveDirection = 2 * moveDir;

    if (playerMoveDirection != 0) {
        if (game.player.x + playerSprite.width + playerMoveDirection >= game.width) {
            game.player.x = game.width - playerSprite.width;
        } else if ((int) game.player.x + playerMoveDirection <= 0) {
            game.player.x = 0;
        } else {
            game.player.x += playerMoveDirection;
        }
    }
}

void processEvents(Game &game, Sprite &playerSprite) {
    if (firePressed && game.numberOfBullets < GAME_MAX_BULLETS) {
        game.bullets[game.numberOfBullets].x = game.player.x + playerSprite.width / 2;
        game.bullets[game.numberOfBullets].y = game.player.y + playerSprite.height;
        game.bullets[game.numberOfBullets].dir = 2;
        ++game.numberOfBullets;
    }
    firePressed = false;
}
