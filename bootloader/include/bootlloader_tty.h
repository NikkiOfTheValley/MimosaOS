// An extremely basic TTY used in the bootloader after calling ExitBootServices,
// as any UEFI TTY functions do not work after calling ExitBootServices

int framebuffer_addr = 0;
int pitch = 0;

// Initilizes the "terminal" with the given framebuffer address and pitch
void terminal_initialize(int _framebuffer_addr, int _pitch)
{
    framebuffer_addr = _framebuffer_addr;
    pitch = _pitch;
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