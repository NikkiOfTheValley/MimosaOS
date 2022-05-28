// An extremely basic TTY used in the bootloader after calling ExitBootServices,
// as any UEFI TTY functions do not work after calling ExitBootServices

#include "font.h"

#include <efidef.h>

EFI_PHYSICAL_ADDRESS framebuffer_addr = 0;
uint32_t pitch = 0;
uint32_t framebuffer_width = 0;
uint32_t framebuffer_height = 0;

int cursorX = 1;
int cursorY = 1;

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
void terminal_initialize(EFI_PHYSICAL_ADDRESS _framebuffer_addr, uint32_t _pitch, uint32_t width, uint32_t height)
{
    framebuffer_addr = _framebuffer_addr;
    pitch = _pitch;
    framebuffer_width = width;
    framebuffer_height = height;

    consoleHeight = framebuffer_height / 8; // Console height is framebuffer height / 8 because the characters are 8 pixels in size
    consoleWidth = framebuffer_width / 8; // Console width is the same
}

// Prints character 'c' at X, Y
void terminal_putc(char c, unsigned int x, unsigned int y, uint32_t fgcolor)
{
    for (unsigned int Y = 0; Y < 8; Y++)
    {
        for (unsigned int X = 0; X < 8; X++)
        {
            if ((font[(c * 8) + Y] & (1 << X)))
            {
                *((uint32_t*)(framebuffer_addr + 4 * pitch * Y + y + 4 * X + x)) = fgcolor;
            }
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

        terminal_putc(c, cursorX * 8, cursorY * 8, 0xFFFFFFFF);

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