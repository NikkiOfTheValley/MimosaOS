#ifndef MEMORY_H
#define MEMORY_H

// Memory header. Includes memory types and will eventually include memory handling functions.

typedef uint64_t physical_address_t;
typedef uint64_t virtual_address_t;

typedef struct {
    uint32_t                      Type;           // Field size is 32 bits followed by 32 bit pad
    uint64_t                      Pad;
    physical_address_t            PhysicalStart;  // Field size is 64 bits
    virtual_address_t             VirtualStart;   // Field size is 64 bits
    uint64_t                      NumberOfPages;  // Field size is 64 bits
    uint64_t                      Attribute;      // Field size is 64 bits
} memory_descriptor_s;
#endif