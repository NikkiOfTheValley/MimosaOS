#include <stddef.h>
#include <stdint.h>

typedef struct framebuffer_info {
    int base_address;
    int width;
    int height;
    int pitch;
} framebuffer_info;

framebuffer_info framebuf;

void draw_byte(uint8_t input, unsigned int X, unsigned int Y)
{
    for (size_t i = 0; i < 8; i++)
    {
        size_t _i = (i * 2) + X;
        size_t _y = Y * 2;
        if (!!((input) & (1 << (i))))
        {
            *((uint32_t*)(framebuf.base_address + 4 * framebuf.pitch * _y + 4 * _i)) = 0xFFFFFFFF;
            *((uint32_t*)(framebuf.base_address + 4 * framebuf.pitch * (_y + 1) + 4 * _i)) = 0xFFFFFFFF;
            *((uint32_t*)(framebuf.base_address + 4 * framebuf.pitch * _y + 4 * (_i + 1))) = 0xFFFFFFFF;
            *((uint32_t*)(framebuf.base_address + 4 * framebuf.pitch * (_y + 1) + 4 * (_i + 1))) = 0xFFFFFFFF;
        }
        else
        {
            *((uint32_t*)(framebuf.base_address + 4 * framebuf.pitch * _y + 4 * _i)) = 0xAAAAAAAA;
            *((uint32_t*)(framebuf.base_address + 4 * framebuf.pitch * (_y + 1) + 4 * _i)) = 0xAAAAAAAA;
            *((uint32_t*)(framebuf.base_address + 4 * framebuf.pitch * _y + 4 * (_i + 1))) = 0xAAAAAAAA;
            *((uint32_t*)(framebuf.base_address + 4 * framebuf.pitch * (_y + 1) + 4 * (_i + 1))) = 0xAAAAAAAA;
        }
    }
}

int k_main(framebuffer_info framebuffer)
{
    framebuf = framebuffer;
    draw_byte(0b11110000, 200, 200);
}