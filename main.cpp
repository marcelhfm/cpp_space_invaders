//
// Created by Marcel Hofmann on 22.09.23.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <iostream>
#include "structs/structs.h"
#include "sprite/sprite.h"
#include "game/game.h"

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

uint32_t rgbToUint32(uint8_t r, uint8_t g, uint8_t b) {
    return (r << 24) | (g << 16) | (b << 8) | 255;
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

    Sprite playerSprite{};
    playerSprite.width = 11;
    playerSprite.height = 7;
    playerSprite.data = new uint8_t[77]
            {
                    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, // .....@.....
                    0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, // ....@@@....
                    0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, // ....@@@....
                    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, // .@@@@@@@@@.
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // @@@@@@@@@@@
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // @@@@@@@@@@@
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // @@@@@@@@@@@
            };

    Sprite alienSprite0{createAlien0()};
    Sprite alienSprite1{createAlien1()};

    SpriteAnimation *alienAnimation = new SpriteAnimation;

    alienAnimation->loop = true;
    alienAnimation->numberOfFrames = 2;
    alienAnimation->frameDuration = 10;
    alienAnimation->time = 0;

    alienAnimation->frames = new Sprite *[2];
    alienAnimation->frames[0] = &alienSprite0;
    alienAnimation->frames[1] = &alienSprite1;

    Game game{};
    game.width = bufferWidth;
    game.height = bufferHeight;
    game.num_aliens = 55;
    game.aliens = new Alien[game.num_aliens];

    game.player.x = 112 - 5;
    game.player.y = 32;

    game.player.life = 3;

    // Init aliens
    for (size_t yi = 0; yi < 5; ++yi) {
        for (size_t xi = 0; xi < 11; ++xi) {
            game.aliens[yi * 11 + xi].x = 16 * xi + 20;
            game.aliens[yi * 11 + xi].y = 17 * yi + 128;
        }
    }

    uint32_t clearColor = rgbToUint32(0, 128, 0);


    int playerMoveDirection = 1;
    while (!glfwWindowShouldClose(window)) {
        clearBuffer(&buffer, clearColor);

        // Draw
        for (size_t ai = 0; ai < game.num_aliens; ++ai) {
            const Alien &alien = game.aliens[ai];
            size_t currentFrame = alienAnimation->time / alienAnimation->frameDuration;
            const Sprite &sprite = *alienAnimation->frames[currentFrame];
            drawSpriteBuffer(&buffer, sprite, alien.x, alien.y, rgbToUint32(128, 0, 0));
        }

        drawSpriteBuffer(&buffer, playerSprite, game.player.x, game.player.y, rgbToUint32(128, 0, 0));

        //Update animations
        ++alienAnimation->time;
        if (alienAnimation->time == alienAnimation->numberOfFrames * alienAnimation->frameDuration) {
            if (alienAnimation->loop) {
                alienAnimation->time = 0;
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

        // Player movement
        if (game.player.x + playerSprite.width + playerMoveDirection >= game.width - 1) {
            game.player.x = game.width - playerSprite.width - playerMoveDirection - 1;
            playerMoveDirection *= -1;
        } else if ((int) game.player.x + playerMoveDirection <= 0)
        {
            game.player.x = 0;
            playerMoveDirection *= -1;
        } else {
            game.player.x += playerMoveDirection;
        }

        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    glDeleteVertexArrays(1, &fullscreenTriangleVao);

    delete[] alienSprite0.data;
    delete[] alienSprite1.data;
    delete[] alienAnimation->frames;
    delete[] buffer.data;
    delete[] game.aliens;

    return 0;
}