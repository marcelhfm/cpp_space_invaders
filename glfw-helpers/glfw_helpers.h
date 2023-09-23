//
// Created by Marcel Hofmann on 23.09.23.
//

#ifndef CPP_SPACE_INVADERS_GLFW_HELPERS_H
#define CPP_SPACE_INVADERS_GLFW_HELPERS_H

#include <GLFW/glfw3.h>

void keyCallback(__attribute__((unused)) GLFWwindow *window, int key, __attribute__((unused)) int scancode, int action,
                 __attribute__((unused)) int mods);
void errorCallback(__attribute__((unused)) int error, const char *description);
void validateShader(GLuint shader, const char *file = 0);
bool validateProgram(GLuint program);
void printOpenGlVersion();
int setupOpenGl(GLFWwindow *&window, GLuint &bufferTexture, GLuint &fullscreenTriangleVao, GLuint &shaderId, size_t bufferWidth, size_t bufferHeight);

#endif //CPP_SPACE_INVADERS_GLFW_HELPERS_H
