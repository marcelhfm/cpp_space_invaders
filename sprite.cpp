//
// Created by Marcel Hofmann on 22.09.23.
//

#include "buffer.h"
#include "sprite.h"

void drawSpriteBuffer(Buffer *buffer, const Sprite &sprite, size_t x, size_t y, uint32_t color) {
    for (size_t xi = 0; xi < sprite.width; ++xi) {
        for (size_t yi = 0; yi < sprite.height; ++yi) {
            size_t sy = sprite.height - 1 + y - yi;
            size_t sx = x + xi;

            if (sprite.data[yi * sprite.width + xi] && sy < buffer->height && sx < buffer->width) {
                buffer->data[sy * buffer->width + sx] = color;
            }
        }
    }
}