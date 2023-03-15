# Warning! This makefile is not intended to fully compile the OS.
# It is purely for conveneince in compiling the bootloader, as the number of flags are ridiculous.

CFLAGS=-target x86_64-unknown-windows -ffreestanding -fshort-wchar -mno-red-zone -Isrc/gnu-efi/inc -Isrc/gnu-efi/inc/x86_64 -Isrc/gnu-efi/inc/protocol
LDFLAGS=-target x86_64-unknown-windows -nostdlib -Wl,-entry:efi_main -Wl,-subsystem:efi_application -fuse-ld=lld-link

all:
	clang $(CFLAGS) -c -o bootloader.o src/bootloader/bootloader.c
	clang $(CFLAGS) -c -o data.o src/gnu-efi/lib/data.c
	clang $(CFLAGS) -c -o misc.o src/gnu-efi/lib/misc.c
	clang $(CFLAGS) -c -o hand.o src/gnu-efi/lib/hand.c
	clang $(CFLAGS) -c -o print.o src/gnu-efi/lib/print.c
	clang $(CFLAGS) -c -o efirtlib.o src/gnu-efi/lib/runtime/efirtlib.c
	clang $(CFLAGS) -c -o rtdata.o src/gnu-efi/lib/runtime/rtdata.c
	clang $(CFLAGS) -c -o rtstr.o src/gnu-efi/lib/runtime/rtstr.c
	clang $(CFLAGS) -c -o init.o src/gnu-efi/lib/init.c
	clang $(CFLAGS) -c -o guid.o src/gnu-efi/lib/guid.c
	clang $(CFLAGS) -c -o dpath.o src/gnu-efi/lib/dpath.c
	clang $(CFLAGS) -c -o str.o src/gnu-efi/lib/str.c
	clang $(CFLAGS) -c -o error.o src/gnu-efi/lib/error.c
	clang $(CFLAGS) -c -o math.o src/gnu-efi/lib/x86_64/math.c
	clang $(CFLAGS) -c -o console.o src/gnu-efi/lib/console.c
	clang $(CFLAGS) -c -o event.o src/gnu-efi/lib/event.c
	clang $(CFLAGS) -c -o initplat.o src/gnu-efi/lib/x86_64/initplat.c
	clang $(LDFLAGS) -o BOOTX64.EFI bootloader.o data.o misc.o hand.o print.o efirtlib.o init.o rtdata.o rtstr.o guid.o dpath.o str.o error.o math.o console.o initplat.o event.o
	rm *.o