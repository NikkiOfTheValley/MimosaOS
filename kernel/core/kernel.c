#include <stddef.h>
#include <stdint.h>
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

int k_main(framebuffer_info_s framebuffer, memory_descriptor_s* memory_map)
{
    framebuf = framebuffer;
    memMap = memory_map;
    
    draw_byte(0b11110000, 200, 200);
}