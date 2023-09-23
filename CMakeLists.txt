cmake_minimum_required(VERSION 3.26)
project(cpp_space_invaders)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_OSX_ARCHITECTURES "x86_64")

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(cpp_space_invaders
        main.cpp
        sprite/sprite.cpp
        sprite/sprite.h
        game/game.cpp
        game/game.h
        glfw-helpers/glfw_helpers.h
        glfw-helpers/glfw_helpers.h
        glfw-helpers/glfw_helpers.h
        glfw-helpers/glfw_helpers.cpp
        game/game_setup.cpp
        game/game_setup.h
        game/game_logic.h
        game/game_logic.cpp
)
target_link_libraries(cpp_space_invaders glfw OpenGL::GL GLEW::GLEW)