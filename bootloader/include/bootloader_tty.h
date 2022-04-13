// An extremely basic TTY used in the bootloader after calling ExitBootServices,
// as any UEFI TTY functions do not work after calling ExitBootServices

#include "font_bitmask.h"

void* framebuffer_addr = 0;
uint32_t pitch = 0;
int framebuffer_width = 0;
int framebuffer_height = 0;

int cursorX = 0;
int cursorY = 0;

int consoleHeight = -1;
int consoleWidth = -1;

void* font_addr;

size_t strlen(const char * _str)
{
    size_t i = 0;
    while(_str[i++]);
    return i - 1;
}

// Initilizes the "terminal" with the given framebuffer address and pitch
void terminal_initialize(void* _framebuffer_addr, uint32_t _pitch, int width, int height, void* font_address)
{
    framebuffer_addr = _framebuffer_addr;
    pitch = _pitch;
    framebuffer_width = width;
    framebuffer_height = height;

    consoleHeight = framebuffer_height / 8; // Console height is framebuffer height / 8 because the characters are 8 pixels in size
    consoleWidth = framebuffer_width / 8; // Console width is the same

    font_addr = font_address;
}

// Prints character 'c' at x, y
void terminal_putc(char c, int x, int y)
{
    for (int y = 0; y > 8; y++)
    {
        
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

        terminal_putc(c, cursorX * 8, cursorY * 8);

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