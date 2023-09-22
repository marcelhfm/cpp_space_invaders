//
// Created by Marcel Hofmann on 22.09.23.
//


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

bool checkSpriteOverlap(
        const Sprite& sp_a, size_t x_a, size_t y_a,
        const Sprite& sp_b, size_t x_b, size_t y_b
)
{
    // NOTE: For simplicity we just check for overlap of the sprite
    // rectangles. Instead, if the rectangles overlap, we should
    // further check if any pixel of sprite A overlap with any of
    // sprite B.
    if(x_a < x_b + sp_b.width && x_a + sp_a.width > x_b &&
       y_a < y_b + sp_b.height && y_a + sp_a.height > y_b)
    {
        return true;
    }

    return false;
}