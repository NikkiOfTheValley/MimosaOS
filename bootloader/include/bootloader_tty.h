// An extremely basic TTY used in the bootloader after calling ExitBootServices,
// as any UEFI TTY functions do not work after calling ExitBootServices

#include "font.h"

#include <efidef.h>

EFI_PHYSICAL_ADDRESS framebuffer_addr = 0;
uint32_t pitch = 0;
uint32_t framebuffer_width = 0;
uint32_t framebuffer_height = 0;

int cursorX = 20;
int cursorY = 20;

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

    consoleHeight = framebuffer_height / CHARACTER_SIZE; // Console height is in characters, so divide the framebuffer height by the character size
    consoleWidth = framebuffer_width / CHARACTER_SIZE;
}

void terminal_clear()
{
    for (size_t y = 0; y < framebuffer_height; y++)
    {
        for (size_t x = 0; x < framebuffer_width; x++)
        {
            *((uint32_t*)(framebuffer_addr + 4 * pitch * y + 4 * x)) = 0x0;
        }
    }
}


// Prints character 'c' at x, y
void terminal_putc(char c, unsigned int x, unsigned int y, uint32_t fgcolor)
{
    for (size_t Y = 0; Y < CHARACTER_SIZE; Y++)
    {
        for (size_t X = 0; X < CHARACTER_SIZE; X++)
        {
            if ((font[(c * CHARACTER_SIZE) + Y] & (0b10000000 >> X)))
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

        terminal_putc(c, cursorX * (CHARACTER_SIZE * 4), cursorY * (CHARACTER_SIZE * 4), 0xFFFFFFFF);

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