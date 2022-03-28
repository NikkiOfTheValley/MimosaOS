# Compile the bootloader
cd gnu-efi/apps
make
cd ../../

# Rename the resulting EFI file to the correct name for UEFI to see it
mv bootloader/bootloader.efi BOOTX64.EFI

# Create a FAT image containing the correct directories and files for a valid UEFI boot partition
dd if=/dev/zero of=fat.img bs=1k count=1440
mformat -i fat.img -f 1440 ::
mmd -i fat.img ::/EFI
mmd -i fat.img ::/EFI/BOOT
mcopy -i fat.img BOOTX64.EFI ::/EFI/BOOT

# Compile the kernel into a UOSE binary so it can be loaded and executed by the bootloader
x86_64-w64-mingw32-gcc -ffreestanding -Ikernel/inc -c -o kernel.o kernel/kernel.c
x86_64-w64-mingw32-gcc -nostdlib -Wl,-dll -shared -Wl,--subsystem,10 -o kernel.uose kernel.o

# Add the kernel to the FAT image
mcopy -i fat.img kernel.uose ::/