//
// Created by Marcel Hofmann on 22.09.23.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <iostream>
#include "sprite/sprite.h"
#include "game/game.h"

bool gameRunning = false;
int moveDir = 0;
bool firePressed = false;

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    switch (key) {
        case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS) {
                gameRunning = false;
            }
            break;
        case GLFW_KEY_RIGHT:
            if (action == GLFW_PRESS) {
                moveDir += 1;
            } else if (action == GLFW_RELEASE) {
                moveDir -= 1;
            }
            break;
        case GLFW_KEY_LEFT:
            if (action == GLFW_PRESS) {
                moveDir -= 1;
            } else if (action == GLFW_RELEASE) {
                moveDir += 1;
            }
            break;
        case GLFW_KEY_SPACE:
            if (action == GLFW_RELEASE) {
                firePressed = true;
            }
            break;
        default:
            break;
    }
}

void errorCallback(__attribute__((unused)) int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

void validateShader(GLuint shader, const char *file = 0) {
    static const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    GLsizei length = 0;

    glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);

    if (length > 0) {
        printf("Shader %d(%s) compile error: %s\n", shader, (file ? file : ""), buffer);
    }
}

bool validateProgram(GLuint program) {
    static const GLsizei BUFFER_SIZE = 512;
    GLchar buffer[BUFFER_SIZE];
    GLsizei length = 0;

    glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);

    if (length > 0) {
        printf("Program %d link error: %s\n", program, buffer);
        return false;
    }

    return true;
}

void clearBuffer(Buffer *buffer, uint32_t color) {
    for (size_t i = 0; i < buffer->width * buffer->height; ++i) {
        buffer->data[i] = color;
    }
}

void printOpenGlVersion() {
    int glVersion[2] = {-1, 1};
    glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
    glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);

    printf("Using OpenGL: %d.%d\n", glVersion[0], glVersion[1]);
    printf("Renderer used: %s\n", glGetString(GL_RENDERER));
    printf("Shading Language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

int main(int argc, char *argv[]) {
    const size_t bufferWidth = 224;
    const size_t bufferHeight = 256;

    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow *window = glfwCreateWindow(bufferWidth, bufferHeight, "Space Invaders", nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, keyCallback);
    glfwMakeContextCurrent(window);


    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error initializing GLEW.\n");
        glfwTerminate();
        return -1;
    }

    printOpenGlVersion();

    glfwSwapInterval(1);

    glClearColor(1.0, 0.0, 0.0, 1.0);

    Buffer buffer{};
    buffer.width = bufferWidth;
    buffer.height = bufferHeight;
    buffer.data = new uint32_t[buffer.width * buffer.height];

    clearBuffer(&buffer, 0);

    // Create texture for presenting structs to OpenGL
    GLuint buffer_texture;
    glGenTextures(1, &buffer_texture);
    glBindTexture(GL_TEXTURE_2D, buffer_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, buffer.width, buffer.height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
                 buffer.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    // Create vao for generating fullscreen triangle
    GLuint fullscreenTriangleVao;
    glGenVertexArrays(1, &fullscreenTriangleVao);

    // Create shader for displaying structs
    static const char *fragmentShader =
            "\n"
            "#version 330\n"
            "\n"
            "uniform sampler2D structs;\n"
            "noperspective in vec2 TexCoord;\n"
            "\n"
            "out vec3 outColor;\n"
            "\n"
            "void main(void){\n"
            "    outColor = texture(structs, TexCoord).rgb;\n"
            "}\n";

    static const char *vertexShader =
            "\n"
            "#version 330\n"
            "\n"
            "noperspective out vec2 TexCoord;\n"
            "\n"
            "void main(void){\n"
            "\n"
            "    TexCoord.x = (gl_VertexID == 2)? 2.0: 0.0;\n"
            "    TexCoord.y = (gl_VertexID == 1)? 2.0: 0.0;\n"
            "    \n"
            "    gl_Position = vec4(2.0 * TexCoord - 1.0, 0.0, 1.0);\n"
            "}\n";

    GLuint shaderId = glCreateProgram();

    {
        //Create vertex shader
        GLuint shaderVp = glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(shaderVp, 1, &vertexShader, 0);
        glCompileShader(shaderVp);
        validateShader(shaderVp, vertexShader);
        glAttachShader(shaderId, shaderVp);

        glDeleteShader(shaderVp);
    }

    {
        //Create fragment shader
        GLuint shaderFp = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(shaderFp, 1, &fragmentShader, 0);
        glCompileShader(shaderFp);
        validateShader(shaderFp, fragmentShader);
        glAttachShader(shaderId, shaderFp);

        glDeleteShader(shaderFp);
    }

    glLinkProgram(shaderId);

    if (!validateProgram(shaderId)) {
        fprintf(stderr, "Error while validating shader.\n");
        glfwTerminate();
        glDeleteVertexArrays(1, &fullscreenTriangleVao);
        delete[] buffer.data;
        return -1;
    }

    glUseProgram(shaderId);

    GLint location = glGetUniformLocation(shaderId, "structs");
    glUniform1i(location, 0);

    //OpenGL setup
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(fullscreenTriangleVao);

    // Prepare game
    Sprite playerSprite{createPlayer()};

    Sprite *alienSprites{createAlienSprites()};
    Sprite deathSprite{createDeathAlien()};

    Sprite bulletSprite{createBullet()};

    // Alien animation
    SpriteAnimation alienAnimation[3];

    for (size_t i = 0; i < 3; ++i) {
        alienAnimation[i].loop = true;
        alienAnimation[i].numberOfFrames = 2;
        alienAnimation[i].frameDuration = 10;
        alienAnimation[i].time = 0;

        alienAnimation[i].frames = new Sprite *[2];
        alienAnimation[i].frames[0] = &alienSprites[2 * i];
        alienAnimation[i].frames[1] = &alienSprites[2 * i + 1];
    }

    Game game{};
    game.width = bufferWidth;
    game.height = bufferHeight;
    game.numberOfAliens = 55;
    game.aliens = new Alien[game.numberOfAliens];

    game.player.x = 112 - 5;
    game.player.y = 32;
    game.player.life = 3;


    // Init aliens
    for(size_t yi = 0; yi < 5; ++yi)
    {
        for(size_t xi = 0; xi < 11; ++xi)
        {
            Alien& alien = game.aliens[yi * 11 + xi];
            alien.type = (5 - yi) / 2 + 1;

            const Sprite& sprite = alienSprites[2 * (alien.type - 1)];

            alien.x = 16 * xi + 20 + (deathSprite.width - sprite.width)/2;
            alien.y = 17 * yi + 128;
        }
    }

    auto* deathCounters = new uint8_t[game.numberOfAliens];
    for (size_t i = 0; i < game.numberOfAliens; i++) {
        deathCounters[i] = 10;
    }

    uint32_t clearColor = rgbToUint32(0, 128, 0);

    gameRunning = true;

    int playerMoveDirection = 0;
    while (!glfwWindowShouldClose(window) && gameRunning) {
        clearBuffer(&buffer, clearColor);

        // Draw
        for (size_t ai = 0; ai < game.numberOfAliens; ++ai) {
            if(!deathCounters[ai]) continue;

            const Alien &alien = game.aliens[ai];

            if (alien.type == ALIEN_DEAD) {
                drawSpriteBuffer(&buffer, deathSprite, alien.x, alien.y, rgbToUint32(128, 0, 0));
            } else {
                const SpriteAnimation &animation = alienAnimation[alien.type - 1];
                size_t currentFrame = alienAnimation->time / alienAnimation->frameDuration;
                const Sprite &sprite = *alienAnimation->frames[currentFrame];
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
        for(auto & i : alienAnimation)
        {
            ++i.time;
            if(i.time == i.numberOfFrames * i.frameDuration)
            {
                i.time = 0;
            }
        }


        glTexSubImage2D(
                GL_TEXTURE_2D, 0, 0, 0,
                buffer.width, buffer.height,
                GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
                buffer.data
        );
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glfwSwapBuffers(window);

        // Simulate Aliens
        for (size_t ai = 0; ai < game.numberOfAliens; ++ai)
        {
            const Alien& alien = game.aliens[ai];
            if (alien.type == ALIEN_DEAD && deathCounters[ai]) {
                --deathCounters[ai];
            }
        }

        // Simulate bullets
        for (size_t bi = 0; bi < game.numberOfBullets;) {
            game.bullets[bi].y += game.bullets[bi].dir;
            if (game.bullets[bi].y >= game.height || game.bullets[bi].y < bulletSprite.height) {
                game.bullets[bi] = game.bullets[game.numberOfBullets - 1];
                --game.numberOfBullets;
                continue;
            }

            // Check for a hit
            for (size_t ai = 0; ai < game.numberOfAliens; ai++)
            {
                const Alien& alien = game.aliens[ai];
                if (alien.type == ALIEN_DEAD) continue;

                const SpriteAnimation& animation = alienAnimation[alien.type -1];
                size_t currentFrame = animation.time / animation.frameDuration;
                const Sprite& alienSprite = *animation.frames[currentFrame];
                bool overlap = checkSpriteOverlap(bulletSprite, game.bullets[bi].x, game.bullets[bi].y, alienSprite, alien.x, alien.y);
                if (overlap)
                {
                    game.aliens[ai].type = ALIEN_DEAD;
                    game.aliens[ai].x -= (deathSprite.width - alienSprite.width) / 2;
                    game.bullets[bi] = game.bullets[game.numberOfBullets -1];
                    --game.numberOfBullets;
                    continue;
                }
            }

            ++bi;
        }

        // Simulate player
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

        // Process events
        if (firePressed && game.numberOfBullets < GAME_MAX_BULLETS) {
            game.bullets[game.numberOfBullets].x = game.player.x + playerSprite.width / 2;
            game.bullets[game.numberOfBullets].y = game.player.y + playerSprite.height;
            game.bullets[game.numberOfBullets].dir = 2;
            ++game.numberOfBullets;
        }
        firePressed = false;

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