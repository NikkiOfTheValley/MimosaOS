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

# Compile the kernel into a ELF binary so it can be loaded and executed by the bootloader
gcc -ffreestanding -Ikernel/include -c -o kernel.o kernel/kernel.c
gcc -nostdlib -nostartfiles -Wl,-e"k_main" -o kernel.elf kernel.o

# Add the kernel to the FAT image
mcopy -i fat.img kernel.elf ::/kernel.elf

# Clean up files
rm kernel.o
rm kernel.elf
rm BOOTX64.EFI