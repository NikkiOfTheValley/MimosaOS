uint32_t pageDirectory[1024] __attribute__((aligned(4096)));

uint32_t firstPageTable[1024] __attribute__((aligned(4096)));


// Initilize page directory
void initPageDir()
{
    // Set each entry to not present
    for (int i = 0; i < 1024; i++)
    {
        // Flags set:
        //  Supervisor: Only kernel-mode can access the page
        //  Write Enabled: Can be both read from and written to
        //  Not Present: This page is not present
        pageDirectory[i] = 0x00000002;
    }
}

void initPage()
{
    // Map pages to the beginning of physical memory
    unsigned int i;
    
    // Fill all entries, mapping 4 megabytes
    for(i = 0; i < 1024; i++)
    {
        // As the address is page aligned, it will always leave 12 bits zeroed.
        // Those bits are used by the attributes
        firstPageTable[i] = (i * 0x1000) | 3; // Attributes: supervisor level, read/write, present.
    }

    // Attributes: supervisor level, read/write, present
    pageDirectory[0] = ((unsigned int)firstPageTable[0]) | 3;
}

//extern void loadPageDir(unsigned int*);