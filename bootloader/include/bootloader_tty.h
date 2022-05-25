// An extremely basic TTY used in the bootloader after calling ExitBootServices,
// as any UEFI TTY functions do not work after calling ExitBootServices

#include "font.h"

uint64_t* framebuffer_addr = 0;
uint32_t pitch = 0;
int framebuffer_width = 0;
int framebuffer_height = 0;

int cursorX = 0;
int cursorY = 0;

int consoleHeight = -1;
int consoleWidth = -1;

size_t strlen(const char * _str)
{
    // I know this isn't the most optimized way to do strlen, but I don't really care as this'll only be used a few times in the bootloader
    // on modern hardware, so it doesn't really matter much.
    size_t i = 0;
    while(_str[i++]);
    return i - 1;
}

// Initilizes the "terminal" with the given framebuffer address and pitch
void terminal_initialize(uint64_t* _framebuffer_addr, uint32_t _pitch, int width, int height)
{
    framebuffer_addr = _framebuffer_addr;
    pitch = _pitch;
    framebuffer_width = width;
    framebuffer_height = height;

    consoleHeight = framebuffer_height / 8; // Console height is framebuffer height / 8 because the characters are 8 pixels in size
    consoleWidth = framebuffer_width / 8; // Console width is the same
}

// Prints character 'c' at X, Y
void terminal_putc(char c, int X, int Y)
{
    // Character image pointer
    const void* char_img_addr = &font.pixel_data[c * characterWidth * font.bytes_per_pixel];

    for (int y = 0; y > 8; y++)
    {
        for (int x = 0; x > 8; x++)
        {
            uint32_t char_pixel = (*((uint32_t*)char_img_addr + x + (y * font.width)) << 8) & 0xFF000000;

            //*((uint32_t*)((uint64_t)framebuffer_addr * pitch * y + Y * x + X)) = char_pixel;
            *(uint64_t*)(framebuffer_addr + (x + X + (y + Y * framebuffer_width))) = char_pixel;
        }
    }
}

// Writes the string `data` of length `length` to the "terminal"
void terminal_write(const char* data, size_t length)
{
    if (consoleWidth == -1 || consoleWidth == -1)
    {
        // Console hasn't been initilized, display error
        // TODO: Actually display error
        return;
    }

    for (size_t i = 0; i < length; i++)
    {
        char c = data[i];

        if (c == '\n')
        {
            cursorY++;
            break;
        }

        terminal_putc(c, cursorX * 8 + 100, cursorY * 8+ 100);

        cursorX++;

        if (cursorX > consoleWidth)
        {
            cursorX = 0;
            cursorY++;
        }
    }
    
}

// Writes the string `data` to the "terminal"
void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}