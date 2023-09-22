//
// Created by Marcel Hofmann on 22.09.23.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <iostream>
#include "buffer.h"
#include "sprite.h"

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


    glClearColor(1.0, 0.0, 0.0, 1.0);

    Buffer buffer{};
    buffer.width = bufferWidth;
    buffer.height = bufferHeight;
    buffer.data = new uint32_t[buffer.width * buffer.height];

    clearBuffer(&buffer, 0);

    // Create texture for presenting buffer to OpenGL
    GLuint buffer_texture;
    glGenTextures(1, &buffer_texture);
    glBindTexture(GL_TEXTURE_2D, buffer_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, buffer.width, buffer.height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, buffer.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    // Create vao for generating fullscreen triangle
    GLuint fullscreenTriangleVao;
    glGenVertexArrays(1, &fullscreenTriangleVao);

    // Create shader for displaying buffer
    static const char* fragmentShader =
            "\n"
            "#version 330\n"
            "\n"
            "uniform sampler2D buffer;\n"
            "noperspective in vec2 TexCoord;\n"
            "\n"
            "out vec3 outColor;\n"
            "\n"
            "void main(void){\n"
            "    outColor = texture(buffer, TexCoord).rgb;\n"
            "}\n";

    static const char* vertexShader =
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
        // Create Vertex shader
        GLuint shaderVp = glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(shaderVp, 1, &vertexShader, nullptr);
        glCompileShader(shaderVp);
        validateShader(shaderVp, vertexShader);
        glAttachShader(shaderId, shaderVp);
    }

    {
        // Create fragment shader
        GLuint shaderFp = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(shaderFp, 1, &fragmentShader, nullptr);
        glCompileShader(shaderFp);
        validateShader(shaderFp, fragmentShader);
        glAttachShader(shaderId, shaderFp);
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

    GLint location = glGetUniformLocation(shaderId, "buffer");
    glUniform1i(location, 0);

    //OpenGl Setup
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(fullscreenTriangleVao);

    //Prepare game
    Sprite alienSprite{};
    alienSprite.width = 11;
    alienSprite.height = 8;
    alienSprite.data = new uint8_t[88]
            {
                    0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0,            // ..@.....@..
                    0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,    // ...@...@...
                    0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0,    // ..@@@@@@@..
                    0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, // .@@.@@@.@@.
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // @@@@@@@@@@@
                    1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, // @.@@@@@@@.@
                    1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, // @.@.....@.@
                    0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0  // ...@@.@@...ﬂ
            };

    uint32_t clearColor = rgbToUint32(0, 128, 0);

    while (!glfwWindowShouldClose(window)) {
        clearBuffer(&buffer, clearColor);

        drawSpriteBuffer(&buffer, alienSprite, 112, 128, rgbToUint32(128, 0, 0));

        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, bufferWidth, bufferHeight, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
                        buffer.data);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}