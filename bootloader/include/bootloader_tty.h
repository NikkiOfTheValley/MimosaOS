// An extremely basic TTY used in the bootloader after calling ExitBootServices,
// as any UEFI TTY functions do not work after calling ExitBootServices

#include "font.h"

int framebuffer_addr = 0;
int pitch = 0;
int framebuffer_width = 0;
int framebuffer_height = 0;

int cursorX = 0;
int cursorY = 0;

int consoleHeight = -1;
int consoleWidth = -1;

// Initilizes the "terminal" with the given framebuffer address and pitch
void terminal_initialize(int _framebuffer_addr, int _pitch, int width, int height)
{
    framebuffer_addr = _framebuffer_addr;
    pitch = _pitch;
    framebuffer_width = width;
    framebuffer_height = height;

    consoleHeight = framebuffer_height / 8; // Console height is framebuffer height / 8 because the characters are 8 pixels in size
    consoleWidth = framebuffer_width / 8; // Console width is the same
}

// Draws character `c` at `x`, `y`, with color `color`
void terminal_putchar(unsigned char c, int x, int y, unsigned int color)
{
    uint32_t glyph[8][8] = font[c];

    glyph = glyph;
}

// Writes the string `data` of length `length` to the "terminal"
void terminal_write(const char* data, size_t length)
{
    if (consoleWidth == -1 || consoleWidth == -1)
    {
        // Console hasn't been initilized, display error
        // TODO: Actually display error
    }

    for (size_t i = 0; i < length; i++)
    {
        char c = data[i];

        if (c == '\n')
        {
            cursorY++;
        }



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