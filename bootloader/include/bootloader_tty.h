// An extremely basic TTY used in the bootloader after calling ExitBootServices,
// as any UEFI TTY functions do not work after calling ExitBootServices

// Font definition
uint64_t font[256] = {
((uint64_t)0b10000000 << 0) |
((uint64_t)0b01000000 << 8) |
((uint64_t)0b00100000 << 16) |
((uint64_t)0b00010000 << 24) |
((uint64_t)0b00001000 << 32) |
((uint64_t)0b00000100 << 40) |
((uint64_t)0b00000010 << 48) |
((uint64_t)0b00000001 << 56)
};

int framebuffer_addr = 0;
int pitch = 0;
int framebuffer_width = 0;
int framebuffer_height = 0;

int cursorX = 0;
int cursorY = 0;

// Initilizes the "terminal" with the given framebuffer address and pitch
void terminal_initialize(int _framebuffer_addr, int _pitch, int width, int height)
{
    framebuffer_addr = _framebuffer_addr;
    pitch = _pitch;
    framebuffer_width = width;
    framebuffer_height = height;
}

// Draws character `c` at `x`, `y`, with color `color`
void terminal_putchar(unsigned char c, unsigned int x, unsigned int y, unsigned int color)
{

}

// Writes the string `data` of length `length` to the "terminal"
void terminal_write()
{

}

// Writes the string `data` to the "terminal"
void terminal_writestring()
{

}