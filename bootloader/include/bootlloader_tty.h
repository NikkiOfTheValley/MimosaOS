// I'm aware that this is a really bad way of doing this. (Pertaining to the libb (lib bootloader))
// I have no idea how I would modify the gnu-efi makefiles to make it link to a static 
// binary though, so this is the only way I can figure out how to do this. 
// If anyone can tell me how to do this, (or at least point me in the right direction) please open an issue so I can fix this.
#include <../../libb/inc> 

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