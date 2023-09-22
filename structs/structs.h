//
// Created by Marcel Hofmann on 22.09.23.
//

#ifndef CPP_SPACE_INVADERS_BUFFER_H
#define CPP_SPACE_INVADERS_BUFFER_H

#include <cstdio>
#include <cstdint>

struct Buffer {
    size_t width, height;
    uint32_t *data;
};

struct Alien {
    size_t x, y;
    uint8_t type;
};

struct Player {
    size_t x, y;
    size_t life;
};

#endif //CPP_SPACE_INVADERS_BUFFER_H
