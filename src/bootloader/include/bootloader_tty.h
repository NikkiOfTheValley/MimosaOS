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

size_t strlen(const char* str)
{
    // I know this isn't the most optimized way to do strlen, but I don't really care as this'll only be used a few times in the bootloader
    // on modern hardware, so it doesn't really matter much.
    size_t i = 0;
    while(str[i++]);
    return i - 1;
}

char* strcpy(char* dest, const char* src)
{
    // Error if src and dest overlap
    if (src + strlen(src) > dest || src < dest) { return NULL; }

    char* temp = dest;
    while((*dest++ = *src++) != '\0');
    return temp;
}

bool strcat(uint32_t num_args, char* buf, size_t size, ...)
{
    size_t strsize = 0;
    va_list ap;
    va_start(ap, num_args);

    for (size_t i = 0; i < num_args; i++)
    {
        strsize += strlen(va_arg(ap, char*));
    }

    strsize = 0;
    va_start(ap, num_args);
    for (size_t i = 0; i < num_args; i++)
    {
        char* s = va_arg(ap, char*);
        strcpy(buf + strsize, s);
        strsize += strlen(s);
    }
    va_end(ap);
    buf[strsize] = '\0';
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
                *((uint32_t*)(framebuffer_addr + 4 * pitch * (Y + y) + 4 * (X + x))) = fgcolor;
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
            cursorX = 1;
            break;
        }

        terminal_putc(c, cursorX * (CHARACTER_SIZE), cursorY * (CHARACTER_SIZE), 0xFFFFFFFF);

        cursorX++;

        if (cursorX > consoleWidth)
        {
            cursorX = 1;
            cursorY++;
        }
    }
    
}

// Writes the string `data` to the "terminal"
void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}

uint64_t num_digits(uint64_t i, uint32_t base)
{
    uint64_t digits = 1;
    uint64_t power = 1;

    while(i / power >= base)
    {
        ++digits;
        power *= base;
    }
    return digits;
}

// Converts the given integer to a string-encoded hex address. Returns a negative integer and does not write to buf if failed.
int to_hex_str(uint64_t n, char* buf, size_t size)
{
    uint64_t digits = num_digits(n, 16);
    uint64_t i = digits - 1;

    // The added digit is to account for the null terminator
    if (size < digits + 1) { return -1; }
    
    while (n > 0)
    {
        uint8_t mod = n % 16;

        if (mod >= 10)
            buf[i--] = (mod - 10) + 'A';
        else
            buf[i--] = mod + '0';
        
        n /= 16;
    }
    buf[size - 1] = '\0';
    return 1;
}