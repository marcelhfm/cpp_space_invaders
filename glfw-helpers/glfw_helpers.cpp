//
// Created by Marcel Hofmann on 23.09.23.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>


extern bool gameRunning;
extern int moveDir;
extern bool firePressed;

void keyCallback(__attribute__((unused)) GLFWwindow *window, int key, __attribute__((unused)) int scancode, int action,
                 __attribute__((unused)) int mods) {
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

void validateShader(GLuint shader, const char *file = nullptr) {
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

void printOpenGlVersion() {
    int glVersion[2] = {-1, 1};
    glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
    glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);

    printf("Using OpenGL: %d.%d\n", glVersion[0], glVersion[1]);
    printf("Renderer used: %s\n", glGetString(GL_RENDERER));
    printf("Shading Language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

int setupOpenGl(GLFWwindow *&window, GLuint &bufferTexture, GLuint &fullscreenTriangleVao, GLuint &shaderId, size_t bufferWidth, size_t bufferHeight) {
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW.\n");
        return -1;
    }

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow((int)bufferWidth, (int)bufferHeight, "Space Invaders", nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        fprintf(stderr, "Failed to create GLFW window.\n");
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

    glfwSwapInterval(1);
    glClearColor(1.0, 0.0, 0.0, 1.0);

    // Create texture for presenting structs to OpenGL
    glGenTextures(1, &bufferTexture);
    glBindTexture(GL_TEXTURE_2D, bufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, (int)bufferWidth, (int)bufferHeight, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
                 nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Create vao for generating fullscreen triangle
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

    shaderId = glCreateProgram();

    {
        //Create vertex shader
        GLuint shaderVp = glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(shaderVp, 1, &vertexShader, nullptr);
        glCompileShader(shaderVp);
        validateShader(shaderVp, vertexShader);
        glAttachShader(shaderId, shaderVp);

        glDeleteShader(shaderVp);
    }

    {
        //Create fragment shader
        GLuint shaderFp = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(shaderFp, 1, &fragmentShader, nullptr);
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
        return -1;
    }

    glUseProgram(shaderId);

    GLint location = glGetUniformLocation(shaderId, "structs");
    glUniform1i(location, 0);


    //OpenGL setup
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(fullscreenTriangleVao);

    printOpenGlVersion();

    return 0;
}