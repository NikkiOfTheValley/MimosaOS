#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "memory.h"
#include "uefi.h"

typedef struct {
    physical_address_t base_address;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
} framebuffer_info_s;

framebuffer_info_s framebuf;

// Memory descriptor from UEFI
memory_descriptor_s* memMap;

// Memory map details
uintn_t mapSize = 0;
uintn_t mapKey = 0;
uintn_t descriptorSize = 0;

void draw_byte(uint8_t input, unsigned int X, unsigned int Y)
{
    for (size_t i = 0; i < 8; i++)
    {
        size_t x = (i * 2) + X;
        size_t y = Y * 2;
        if (!!((input) & (1 << (i))))
        {
            *((uint32_t*)(framebuf.base_address + 4 * framebuf.pitch * y + 4 * x)) = 0xFFFFFFFF;
            *((uint32_t*)(framebuf.base_address + 4 * framebuf.pitch * (y + 1) + 4 * x)) = 0xFFFFFFFF;
            *((uint32_t*)(framebuf.base_address + 4 * framebuf.pitch * y + 4 * (x + 1))) = 0xFFFFFFFF;
            *((uint32_t*)(framebuf.base_address + 4 * framebuf.pitch * (y + 1) + 4 * (x + 1))) = 0xFFFFFFFF;
        }
        else
        {
            *((uint32_t*)(framebuf.base_address + 4 * framebuf.pitch * y + 4 * x)) = 0xAAAAAAAA;
            *((uint32_t*)(framebuf.base_address + 4 * framebuf.pitch * (y + 1) + 4 * x)) = 0xAAAAAAAA;
            *((uint32_t*)(framebuf.base_address + 4 * framebuf.pitch * y + 4 * (x + 1))) = 0xAAAAAAAA;
            *((uint32_t*)(framebuf.base_address + 4 * framebuf.pitch * (y + 1) + 4 * (x + 1))) = 0xAAAAAAAA;
        }
    }
}

int k_main(framebuffer_info_s framebuffer, memory_descriptor_s* memory_map)
{
    framebuf = framebuffer;
    memMap = memory_map;

    *((uint32_t*)(framebuf.base_address + 4 * framebuf.pitch * 20 + 4 * 20)) = 0xFFFFFFFF;
    *((uint32_t*)(framebuf.base_address + 4 * framebuf.pitch * 20 + 4 * 21)) = 0xFFFFFFFF;
    *((uint32_t*)(framebuf.base_address + 4 * framebuf.pitch * 20 + 4 * 22)) = 0xFFFFFFFF;
    *((uint32_t*)(framebuf.base_address + 4 * framebuf.pitch * 20 + 4 * 23)) = 0xFFFFFFFF;
    *((uint32_t*)(framebuf.base_address + 4 * framebuf.pitch * 21 + 4 * 20)) = 0xFFFFFFFF;
    *((uint32_t*)(framebuf.base_address + 4 * framebuf.pitch * 22 + 4 * 21)) = 0xFFFFFFFF;
    *((uint32_t*)(framebuf.base_address + 4 * framebuf.pitch * 23 + 4 * 22)) = 0xFFFFFFFF;
    *((uint32_t*)(framebuf.base_address + 4 * framebuf.pitch * 24 + 4 * 23)) = 0xFFFFFFFF;

    draw_byte(0b00000000, 16, 20);
    draw_byte(0b00100000, 16, 22);
    draw_byte(0b00001000, 16, 24);
    draw_byte(0b01110110, 16, 26);
    draw_byte(0b11001110, 16, 28);
    draw_byte(0b10010011, 16, 30);
    draw_byte(0b10110101, 16, 32);
    draw_byte(0b10011000, 16, 34);

    while(true) {}
}