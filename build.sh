# Compile the bootloader
make

rm fat.img

# Create a FAT image containing the correct directories and files for a valid UEFI boot partition
dd if=/dev/zero of=fat.img bs=1k count=20000
mformat -i fat.img ::
mmd -i fat.img ::/EFI
mmd -i fat.img ::/EFI/BOOT
mcopy -i fat.img BOOTX64.EFI ::/EFI/BOOT

# Compile the kernel into a freestanding binary so it can be loaded and executed by the bootloader

# RUSTFLAGS="-C relocation-model=pie"

cargo +nightly build --release --target x86_64-unknown-none

# Convert ELF into flat binary
# cd target/x86_64-unknown-none/release/
# mv kernel kernel.elf
# objcopy -I elf64-x86-64 -O binary kernel.elf kernel.bin
# cd ../../../

# Add the kernel to the FAT image
mcopy -i fat.img target/x86_64-unknown-none/release/kernel ::/kernel.elf

#gcc -c src/kernel/core/test.s
#ld test.o --oformat=binary

#mcopy -i fat.img a.out ::/kernel.elf

# Clean up files
rm BOOTX64.EFI
rm a.out
rm test.o