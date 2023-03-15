#![no_std]
#![no_main]

use core::panic::PanicInfo;

#[repr(C)]
pub struct FramebufferInfo {
    base_address: u64,
    width: u32,
    height: u32,
    pitch: u32
}

#[repr(C)]
pub struct MemoryDescriptor {
    r#type: u32,           // Field size is 32 bits followed by 32 bit pad
    pad: u32,
    physical_start: u64,  // Field size is 64 bits
    virtual_start: u64,   // Field size is 64 bits
    number_of_pages: u64,  // Field size is 64 bits
    attribute: u64       // Field size is 64 bits
}

#[no_mangle]
pub extern "C" fn _start(framebuffer_info: FramebufferInfo, _memory_map: *const MemoryDescriptor) -> ! {
    let framebuffer = framebuffer_info.base_address as *mut u32;
    let pitch = framebuffer_info.pitch;
    
    // Clear screen
    for y in 0..framebuffer_info.height {
        for x in 0..framebuffer_info.width {
            unsafe {
                *framebuffer.offset((x * pitch + y) as isize) = 0x00000000;
            }
        }
    }

    unsafe {
        *framebuffer.offset((5 * pitch + 5) as isize) = 0xFFFFFFFF;
        *framebuffer.offset((6 * pitch + 5) as isize) = 0xFFFFFFFF;
        *framebuffer.offset((7 * pitch + 5) as isize) = 0xFFFFFFFF;
        *framebuffer.offset((8 * pitch + 5) as isize) = 0xFFFFFFFF;
        *framebuffer.offset((9 * pitch + 5) as isize) = 0xFFFFFFFF;
        *framebuffer.offset((10 * pitch + 5) as isize) = 0xFFFFFFFF;
        *framebuffer.offset((11 * pitch + 5) as isize) = 0xFFFFFFFF;
        *framebuffer.offset((12 * pitch + 5) as isize) = 0xFFFFFFFF;
    }

    loop {}
}

/// This function is called on panic.
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}
