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
pub extern "C" fn _start(_framebuffer_info: FramebufferInfo, _memory_map: *const MemoryDescriptor) -> ! {
    //let framebuffer = framebuffer_info.base_address as *mut u32;
    //let pitch = framebuffer_info.pitch;

    // Clear screen
    //for y in 0..framebuffer_info.height {
    //    for x in 0..framebuffer_info.width {
    //        unsafe {

    //            *framebuffer.offset((pitch * y + x) as isize) = 0x00000000;
    //        }
    //    }
    //}

    //unsafe {
    //    *framebuffer.offset((pitch * 20 + 1) as isize) = 0xFFFFFFFF;
    //    *framebuffer.offset((pitch * 20 + 2) as isize) = 0xFFFFFFFF;
    //    *framebuffer.offset((pitch * 20 + 3) as isize) = 0xFFFFFFFF;
    //    *framebuffer.offset((pitch * 20 + 4) as isize) = 0xFFFFFFFF;
    //    *framebuffer.offset((pitch * 20 + 5) as isize) = 0xFFFFFFFF;
    //    *framebuffer.offset((pitch * 20 + 6) as isize) = 0xFFFFFFFF;
    //    *framebuffer.offset((pitch * 20 + 7) as isize) = 0xFFFFFFFF;
    //    *framebuffer.offset((pitch * 20 + 8) as isize) = 0xFFFFFFFF;
    //}

    loop {}
}

/// This function is called on panic.
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}
