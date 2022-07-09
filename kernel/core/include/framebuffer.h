#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

typedef struct {
    physical_address_t base_address;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
} framebuffer_info_s;

#endif