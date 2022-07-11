#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "memory.h"
#include "uefi.h"
#include "framebuffer.h"
#include "tty.h"

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

    terminal_initialize(framebuf.base_address, framebuf.pitch, framebuf.width, framebuf.height);

    terminal_writestring("Hello (kernel) world!");

    while(true) {}
}