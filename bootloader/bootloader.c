#include <efi.h>
#include <efilib.h>
#include <stddef.h>
#include <stdbool.h>
#include "include/file_handling.h"
#include "include/bootloader_tty.h"

#if 0
extern void dump_stack(void);
__asm(".globl	dump_stack\n"
	"dump_stack:\n"
	"	movq %rsp, %rdi\n"
	"	jmp *dump_stack_helper@GOTPCREL(%rip)\n"
	".size	dump_stack, .-dump_stack");

void dump_stack_helper(uint64_t rsp_val)
{
	uint64_t *rsp = (uint64_t *)rsp_val;
	int x;

	Print(L"%%rsp: 0x%08x%08x stack:\r\n",
					(rsp_val & 0xffffffff00000000) >>32,
					 rsp_val & 0xffffffff);
	for (x = 0; x < 8; x++) {
		Print(L"%08x: ", ((uint64_t)rsp) & 0xffffffff);
		Print(L"%016x ", *rsp++);
		Print(L"%016x ", *rsp++);
		Print(L"%016x ", *rsp++);
		Print(L"%016x\r\n", *rsp++);
	}
}
#endif

// Checks if the actual result matches the expected result
#define ErrorCheck(actual, expected) if(actual != expected) return actual

EFI_STATUS Status;
EFI_INPUT_KEY Key;

EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;


// Waits for user input from the keyboard
void WaitForInput() {
    Print(L"Press any key to continue\r\n");

    Status = ST->ConIn->Reset(ST->ConIn, FALSE);
    if (EFI_ERROR(Status))
        Print(L"EFI_ERROR_UNKNOWN\r\n");
    
    while ((Status = ST->ConIn->ReadKeyStroke(ST->ConIn, &Key)) == EFI_NOT_READY);
}

// Draws a single byte at X, Y
void draw_byte(uint8_t input, unsigned int X, unsigned int Y)
{
    for (size_t i = 0; i < 8; i++)
    {
        size_t _i = (i * 2) + X;
        size_t _y = Y * 2;

        if (!!((input) & (1 << (i))))
        {
            *((uint32_t*)(gop->Mode->FrameBufferBase + 4 * gop->Mode->Info->PixelsPerScanLine * _y + 4 * _i)) = 0xFFFFFFFF;
            *((uint32_t*)(gop->Mode->FrameBufferBase + 4 * gop->Mode->Info->PixelsPerScanLine * (_y + 1) + 4 * _i)) = 0xFFFFFFFF;
            *((uint32_t*)(gop->Mode->FrameBufferBase + 4 * gop->Mode->Info->PixelsPerScanLine * _y + 4 * (_i + 1))) = 0xFFFFFFFF;
            *((uint32_t*)(gop->Mode->FrameBufferBase + 4 * gop->Mode->Info->PixelsPerScanLine * (_y + 1) + 4 * (_i + 1))) = 0xFFFFFFFF;
        }
        else
        {
            *((uint32_t*)(gop->Mode->FrameBufferBase + 4 * gop->Mode->Info->PixelsPerScanLine * _y + 4 * _i)) = 0xAAAAAAAA;
            *((uint32_t*)(gop->Mode->FrameBufferBase + 4 * gop->Mode->Info->PixelsPerScanLine * (_y + 1) + 4 * _i)) = 0xAAAAAAAA;
            *((uint32_t*)(gop->Mode->FrameBufferBase + 4 * gop->Mode->Info->PixelsPerScanLine * _y + 4 * (_i + 1))) = 0xAAAAAAAA;
            *((uint32_t*)(gop->Mode->FrameBufferBase + 4 * gop->Mode->Info->PixelsPerScanLine * (_y + 1) + 4 * (_i + 1))) = 0xAAAAAAAA;
        }
    }
}

typedef struct framebuffer_info {
    EFI_PHYSICAL_ADDRESS base_address;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
} framebuffer_info;


EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);
 
    ST = SystemTable;

    // Some sanity checks

    Status = ST->ConOut->OutputString(ST->ConOut, L"ConOut Sanity Check\n");
    if (EFI_ERROR(Status)) {
        // I'm not sure if this would even get displayed, 
        // as at this point it's been proven that ConOut is somehow broken
        Print(L"Sanity check failed\r\n"); 
        WaitForInput();
        return Status;
    }

    Print(L"Sanity check passed\n");

    Print(L"KEY_IN Sanity Check\n");
    WaitForInput();

    Print(L"Sanity check passed\n");


    Print(L"All sanity checks passed, continuing\n\n");


    Print(L"Locating GOP\n");

    // Locate the GOP
	Status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);
    if(EFI_ERROR(Status)) {
        Print(L"Could not locate GOP: %r\n", Status);
		WaitForInput();
		return Status;
	}
    else {
        Print(L"Located GOP, locating framebuffer\n");
	}

    // GOP is null, print error message
	if (!gop) {
		Print(L"LocateProtocol(GOP, &gop) returned %r but GOP is NULL\n", Status);
		WaitForInput();
		return EFI_UNSUPPORTED;
	}
		
    // Assume framebuffer address is initilized because it is exceedingly unlikely at this point for the GOP to not be valid
    Print(L"Assuming framebuffer address is initilized\n");


    // Get the current mode's info

    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
    UINTN SizeOfInfo, numModes, nativeMode;
 
    Status = uefi_call_wrapper(gop->QueryMode, 4, gop, gop->Mode==NULL?0:gop->Mode->Mode, &SizeOfInfo, &info);
    // This is needed to get the current video mode
    if (Status == EFI_NOT_STARTED)
        Status = uefi_call_wrapper(gop->SetMode, 2, gop, 0);
    if(EFI_ERROR(Status)) {
        Print(L"Unable to get native mode\n");
        WaitForInput();
        return Status;
    } else {
        nativeMode = gop->Mode->Mode;
        numModes = gop->Mode->MaxMode;
    }


    // Get the info for all modes, and print said info for each respective mode

    for (UINTN i = 0; i < numModes; i++) {
        Status = uefi_call_wrapper(gop->QueryMode, 4, gop, i, &SizeOfInfo, &info);
        Print(L"mode %03d width %d height %d format %x%s\n",
            i,
            info->HorizontalResolution,
            info->VerticalResolution,
            info->PixelFormat,
            i == nativeMode ? L"(current)" : L""
        );
    }


    // Print framebuffer information
    Print(L"Framebuffer address %x size %d, width %d height %d pixelsperline %d\n",
        gop->Mode->FrameBufferBase,
        gop->Mode->FrameBufferSize,
        gop->Mode->Info->HorizontalResolution,
        gop->Mode->Info->VerticalResolution,
        gop->Mode->Info->PixelsPerScanLine
    );


    // Get a volume handle to the volume the bootloader has been loaded from

    EFI_LOADED_IMAGE *loaded_image = NULL;                  // Image interface
    EFI_GUID lipGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;      // Image interface GUID
    EFI_FILE_IO_INTERFACE *IOVolume;                        // File system interface
    EFI_GUID fsGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID; // File system interface GUID
    EFI_FILE_HANDLE volumeHandle;                           // The volume's interface
 
    // Get the loaded image protocol interface for the "image"
    uefi_call_wrapper(BS->HandleProtocol, 3, ImageHandle, &lipGuid, (void**) &loaded_image);
    // Get the volume handle
    uefi_call_wrapper(BS->HandleProtocol, 3, loaded_image->DeviceHandle, &fsGuid, (void*)&IOVolume);
    // VolumeHandle = LibOpenRoot(loaded_image->DeviceHandle); // Not sure if this works, so this is commented out
    uefi_call_wrapper(IOVolume->OpenVolume, 2, IOVolume, &volumeHandle);


    EFI_FILE_HANDLE kernelHandle; // Kernel file handle

    // Get a handle to the kernel file
    uefi_call_wrapper(volumeHandle->Open, 5, volumeHandle, &kernelHandle, L"kernel.elf", EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);
    
    // Read from the kernel file and load it into memory

    uint64_t kernelSize = FileSize(kernelHandle);
    uint8_t *kernelBuf = AllocatePool(kernelSize);

    uefi_call_wrapper(kernelHandle->Read, 3, kernelHandle, &kernelSize, kernelBuf);
    

    Print(L"Loaded kernel at address %x\n", kernelBuf);

    // Tell UEFI to shut down any of its services. We're on our own now.

    UINTN mapSize = 0, mapKey, descriptorSize;
    EFI_MEMORY_DESCRIPTOR *memoryMap = NULL;
    UINT32 descriptorVersion;
    // Get the required memory pool size for the memory map
    Status = uefi_call_wrapper(BS->GetMemoryMap, 5, &mapSize, memoryMap, NULL, &descriptorSize, NULL);
    if(Status != EFI_BUFFER_TOO_SMALL) { return Status; }

    // Allocating the pool creates at least one new descriptor for the chunk of memory changed to EfiLoaderData
    // Not sure if UEFI firmware must allocate on a memory type boundary. If not, then two descriptors might be created
    mapSize += 2 * descriptorSize;
    // Get a pool of memory to hold the map
    Status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, mapSize, (void **)&memoryMap);
    if(Status != EFI_SUCCESS) { return Status; }

    // Get the actual memory map
    Status = uefi_call_wrapper(BS->GetMemoryMap, 5, &mapSize, memoryMap, &mapKey, &descriptorSize, &descriptorVersion);
    if(Status != EFI_SUCCESS) { return Status; }

    // And finally exit boot services
    Status = uefi_call_wrapper(BS->ExitBootServices, 2, ImageHandle, mapKey);
    if(Status != EFI_SUCCESS) { return Status; }

    // Initilize the terminal so printing is possible
    terminal_initialize(gop->Mode->FrameBufferBase, gop->Mode->Info->PixelsPerScanLine, gop->Mode->Info->HorizontalResolution, gop->Mode->Info->VerticalResolution);
    terminal_clear();
    
    terminal_writestring("ABCD1234!\n");
    terminal_writestring("ABCD1234!\n");

    // Stop here, as there's a few problems and I'd like to figure out where they're occuring.
    while(true) { }

    // Parse the kernel ELF file
    
    uint32_t header_magic = 0;

    header_magic |= (uint32_t)kernelBuf[0] << 24;
    header_magic |= (uint32_t)kernelBuf[1] << 16;
    header_magic |= (uint32_t)kernelBuf[2] << 8;
    header_magic |= (uint32_t)kernelBuf[3];

    // Debug info
    for (size_t y = 0; y < gop->Mode->Info->VerticalResolution - 5; y++)
    {
        draw_byte(kernelBuf[y], 0, y);
    }

    // More debug info
    draw_byte((uint8_t)(header_magic), 200, 200);
    draw_byte((uint8_t)(header_magic >> 8), 216, 200);
    draw_byte((uint8_t)(header_magic >> 16), 232, 200);
    draw_byte((uint8_t)(header_magic >> 24), 248, 200);

    if (header_magic == 0x6232A2FE) // If header_magic == .ELF
    {
        draw_byte((uint8_t)0b11110000, 300, 300);
    }
    else
    {
        terminal_writestring("Critical Error: Kernel is not an ELF file! Stopping boot!\n");
        terminal_writestring("Please restart your machine. (I am too lazy to implement ACPI drivers)\n");

        // Stop here, so the user can power off without causing problems.
        while (true) { }
    }


    // Initilize a struct with the required framebuffer info to pass to the kernel, so drawing is still possible

    framebuffer_info framebuf;
    framebuf.base_address = gop->Mode->FrameBufferBase;
    framebuf.width = gop->Mode->Info->HorizontalResolution;
    framebuf.height = gop->Mode->Info->VerticalResolution;
    framebuf.pitch = gop->Mode->Info->PixelsPerScanLine;

    // Jump to the kernel address (not actually kernel address, haven't gotten a ELF loader working yet. This is a placeholder.)
    typedef int k_main(framebuffer_info framebuffer);
    k_main* k = (k_main*)0xdeadbeef;
    k(framebuf);
    
    // Framebuffer and data to do with writing to said framebuffer should still be valid, so this should still work.
    terminal_writestring("Unrecoverable Error Detected! Halting!\n");
    terminal_writestring("Please restart your machine. (I am too lazy to implement ACPI drivers)\n");

    // Stop here, so the user can power off without causing problems.
    while(true) {}

    // Kernel shouldn't ever exit, but GCC complains if you leave out the return statement
    return Status;
}
