//
// Created by Marcel Hofmann on 22.09.23.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "sprite/sprite.h"
#include "game/game.h"
#include "glfw-helpers/glfw_helpers.h"
#include "game/game_setup.h"
#include "game/game_logic.h"

bool gameRunning = false;
int moveDir = 0;
bool firePressed = false;
const size_t bufferWidth = 224;
const size_t bufferHeight = 256;


void clearBuffer(Buffer *buffer, uint32_t color) {
    for (size_t i = 0; i < buffer->width * buffer->height; ++i) {
        buffer->data[i] = color;
    }
}

int main() {
    GLFWwindow *window{nullptr};
    GLuint bufferTexture{0};
    GLuint fullscreenTriangleVao{0};
    GLuint shaderId{0};

    int status{setupOpenGl(window, bufferTexture, fullscreenTriangleVao, shaderId, bufferWidth, bufferHeight)};
    if (status == -1) return status;

    Buffer buffer{};
    buffer.width = bufferWidth;
    buffer.height = bufferHeight;
    buffer.data = new uint32_t[buffer.width * buffer.height];

    clearBuffer(&buffer, 0);

    // Prepare game
    Sprite playerSprite{createPlayer()};
    Sprite *alienSprites{createAlienSprites()};
    Sprite deathSprite{createDeathAlien()};
    Sprite bulletSprite{createBullet()};
    Sprite textSpriteSheet{createTextSpriteSheet()};
    Sprite numberSpriteSheet{createNumberSpriteSheet()};
    SpriteAnimation alienAnimation[3];
    Game game{};
    game.numberOfAliens = 55;
    auto *deathCounters = new uint8_t[game.numberOfAliens];


    uint32_t clearColor = rgbToUint32(0, 128, 0);

    size_t score = 0;

    gameRunning = true;

    int playerMoveDirection;
    int alienMoveDirection {1};
    int alienUpdateFrequency {120};
    bool skipSimulation {false};

    prepareGame(game, alienSprites, deathSprite, alienAnimation, deathCounters, buffer, alienUpdateFrequency);


    // Game loop
    while (!glfwWindowShouldClose(window) && gameRunning) {
        clearBuffer(&buffer, clearColor);

        drawScene(buffer, game, alienAnimation, deathCounters, playerSprite, bulletSprite, deathSprite, textSpriteSheet,
                  numberSpriteSheet,
                  reinterpret_cast<uint32_t &>(score));

        glTexSubImage2D(
                GL_TEXTURE_2D, 0, 0, 0,
                (int) buffer.width, (int) buffer.height,
                GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
                buffer.data
        );
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glfwSwapBuffers(window);

        if (!skipSimulation)
        {
        simulateAliens(game, alienAnimation, deathCounters, alienMoveDirection);
        }
        simulateBullets(game, bulletSprite, deathSprite, alienAnimation, reinterpret_cast<uint32_t &>(score));
        simulatePlayer(game, playerSprite, playerMoveDirection);
        processEvents(game, playerSprite);

        skipSimulation = !skipSimulation;
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    glDeleteVertexArrays(1, &fullscreenTriangleVao);

    for (size_t i = 0; i < 6; ++i) {
        delete[] alienSprites[i].data;
    }
    delete[] deathSprite.data;

    for (auto &i: alienAnimation) {
        delete[] i.frames;
    }

    delete[] buffer.data;
    delete[] game.aliens;

    return 0;
}