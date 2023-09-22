//
// Created by Marcel Hofmann on 22.09.23.
//

#ifndef CPP_SPACE_INVADERS_BUFFER_H
#define CPP_SPACE_INVADERS_BUFFER_H

#include <cstdio>
#include <cstdint>

struct Buffer{
    size_t width, height;
    uint32_t* data;
};

#endif //CPP_SPACE_INVADERS_BUFFER_H
