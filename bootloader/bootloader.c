#include <efi.h>
#include <efilib.h>
#include <stddef.h>
#include "include/file_handling.h"
//#include <include/bootloader_tty.h>

#if 0
extern void dump_stack(void);
asm(	".globl	dump_stack\n"
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


// Waits for user input from the keyboard
void WaitForInput() {
    Print(L"Press any key to continue\r\n");

    Status = ST->ConIn->Reset(ST->ConIn, FALSE);
    if (EFI_ERROR(Status))
        Print(L"EFI_ERROR_UNKNOWN\r\n");
    
    while ((Status = ST->ConIn->ReadKeyStroke(ST->ConIn, &Key)) == EFI_NOT_READY);
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);
 
    ST = SystemTable;

    // Some sanity checks

    Status = ST->ConOut->OutputString(ST->ConOut, L"ConOut Sanity Check\r\n");
    if (EFI_ERROR(Status)) {
        // I'm not sure if this would even get displayed, 
        // as at this point it's been proven that ConOut is somehow broken
        Print(L"Sanity check failed\r\n"); 
        
        WaitForInput();
        return Status;
    }

    Print(L"Sanity check passed\r\n");

    Print(L"KEY_IN Sanity Check\r\n");
    WaitForInput();

    Print(L"Sanity check passed\r\n");


    Print(L"All sanity checks passed, continuing\r\n\r\n");

    // Locate the GOP

    Print(L"Locating GOP\r\n");
	
	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;

    // Locate the GOP
	Status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);
    if(EFI_ERROR(Status)) {
        Print(L"Could not locate GOP: %r\r\n", Status);
		WaitForInput();
		return Status;
	}
    else {
        Print(L"Located GOP, locating framebuffer\r\n");
	}

    // GOP is null, print error message
	if (!gop) {
		Print(L"LocateProtocol(GOP, &gop) returned %r but GOP is NULL\r\n", Status);
		WaitForInput();
		return EFI_UNSUPPORTED;
	}
		
    // Assume framebuffer address is initilized because it is exceedingly unlikely at this point for the GOP to not be valid
    Print(L"Assuming framebuffer address is initilized\r\n");


    // Get the current mode's info

    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
    UINTN SizeOfInfo, numModes, nativeMode;
 
    Status = uefi_call_wrapper(gop->QueryMode, 4, gop, gop->Mode==NULL?0:gop->Mode->Mode, &SizeOfInfo, &info);
    // This is needed to get the current video mode
    if (Status == EFI_NOT_STARTED)
        Status = uefi_call_wrapper(gop->SetMode, 2, gop, 0);
    if(EFI_ERROR(Status)) {
        Print(L"Unable to get native mode\r\n");
        WaitForInput();
        return Status;
    } else {
        nativeMode = gop->Mode->Mode;
        numModes = gop->Mode->MaxMode;
    }


    // Get the info for all modes, and print said info for each respective mode

    for (UINTN i = 0; i < numModes; i++) {
        Status = uefi_call_wrapper(gop->QueryMode, 4, gop, i, &SizeOfInfo, &info);
        Print(L"mode %03d width %d height %d format %x%s\r\n",
            i,
            info->HorizontalResolution,
            info->VerticalResolution,
            info->PixelFormat,
            i == nativeMode ? "(current)" : ""
        );
    }


    // Print framebuffer information
    Print(L"Framebuffer address %x size %d, width %d height %d pixelsperline %d\r\n",
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
    uefi_call_wrapper(BS->HandleProtocol, 3, ImageHandle, &lipGuid, (void **) &loaded_image);
    // Get the volume handle
    uefi_call_wrapper(BS->HandleProtocol, 3, loaded_image->DeviceHandle, &fsGuid, (VOID*)&IOVolume);
    // VolumeHandle = LibOpenRoot(loaded_image->DeviceHandle); // Not sure if this works, so this is commented out
    uefi_call_wrapper(IOVolume->OpenVolume, 2, IOVolume, &volumeHandle);


    EFI_FILE_HANDLE kernelHandle; // Kernel file handle

    // Get a handle to the kernel file
    uefi_call_wrapper(volumeHandle->Open, 5, volumeHandle, &kernelHandle, L"kernel.uose", EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);
    

    // Read from the kernel file and load it into memory

    uint64_t kernelSize = FileSize(kernelHandle);
    uint8_t  *kernelBuf = AllocatePool(kernelSize);

    uefi_call_wrapper(kernelHandle->Read, 3, kernelHandle, &kernelSize, kernelBuf);


    // Tell UEFI to shut down any of it's services. We're on our own now.

    UINTN mapSize = 0, mapKey, descriptorSize;
    EFI_MEMORY_DESCRIPTOR *memoryMap = NULL;
    UINT32 descriptorVersion;
    // Get the required memory pool size for the memory map
    Status = uefi_call_wrapper(BS->GetMemoryMap, 5, &mapSize, memoryMap, NULL, &descriptorSize, NULL);
    ErrorCheck(Status, EFI_BUFFER_TOO_SMALL);
    // Allocating the pool creates at least one new descriptor for the chunk of memory changed to EfiLoaderData
    // Not sure if UEFI firmware must allocate on a memory type boundary. If not, then two descriptors might be created
    mapSize += 2 * descriptorSize;
    // Get a pool of memory to hold the map
    Status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, mapSize, (void **)&memoryMap);
    ErrorCheck(Status, EFI_SUCCESS);
    // Get the actual memory map
    Status = uefi_call_wrapper(BS->GetMemoryMap, 5, &mapSize, memoryMap, &mapKey, &descriptorSize, &descriptorVersion);
    ErrorCheck(Status, EFI_SUCCESS);

    // And finally exit boot services
    Status = uefi_call_wrapper(BS->ExitBootServices, 2, ImageHandle, mapKey);
    ErrorCheck(Status, EFI_SUCCESS);


    // Plot 1 white pixel
    *((uint32_t*)(gop->Mode->FrameBufferBase + 4 * gop->Mode->Info->PixelsPerScanLine * 10 + 4 * 10)) = 0xFFFFFFFF;

    // Jump to the kernel address (not actually kernel address, haven't gotten a parser working yet. This is a placeholder.)
    typedef int func(void);
    func* f = (func*)0xdeadbeef;
    f();
    
    return Status;
}
