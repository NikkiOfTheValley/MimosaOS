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
void terminal_putc(char c, int x, int y, int fgcolor)
{
	uint64_t *dest;
	uint64_t *dest32;
	unsigned char *src;
	int row;
	uint32_t fgcolor32;
 
	fgcolor32 = fgcolor | (fgcolor << 8) | (fgcolor << 16) | (fgcolor << 24);
	src = &font[0] + c * 16;
	dest = framebuffer_addr + y * (pitch * 32) + x;
	for(row = 0; row < 8; row++) {
		if(*src != 0) {
			uint32_t mask_low = font_mask[*src][0];
			uint32_t mask_high = font_mask[*src][1];
			dest32 = dest;
			dest32[0] = (dest[0] & ~mask_low) | (fgcolor32 & mask_low);
			dest32[1] = (dest[1] & ~mask_high) | (fgcolor32 & mask_high);
		}
		src++;
		dest += (pitch * 32);
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

        terminal_putc(c, cursorX * 8, cursorY * 8, 0xFF);

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