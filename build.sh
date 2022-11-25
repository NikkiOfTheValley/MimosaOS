# Compile the bootloader
#cd src/gnu-efi/apps
#make
#cd ../../../

#cd src/bootloader
#clang bootloader.c --target=x86_64-windows -I"../gnu-efi/inc" -I"../gnu-efi/inc/x86_64" -I"../gnu-efi/inc/protocol" -ffreestanding -nostdlib -mno-stack-arg-probe -mgeneral-regs-only -fuse-ld=lld -Wl,-entry:efi_main -Wl,-subsystem:efi_application -Wl,-largeaddressaware
#cd ../../

make

# Create a FAT image containing the correct directories and files for a valid UEFI boot partition
dd if=/dev/zero of=fat.img bs=1k count=20000
mformat -i fat.img ::
mmd -i fat.img ::/EFI
mmd -i fat.img ::/EFI/BOOT
mcopy -i fat.img BOOTX64.EFI ::/EFI/BOOT

# Compile the kernel into a ELF binary so it can be loaded and executed by the bootloader

cargo build --release

# Add the kernel to the FAT image
mcopy -i fat.img target/x86_64-unknown-none/release/kernel ::/kernel.elf

# Clean up files
rm BOOTX64.EFI