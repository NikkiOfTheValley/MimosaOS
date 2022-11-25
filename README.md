This is a OS I'm working on for fun and to learn about OS development.

This is *not* intended as a daily driver. (and is currently impossible to use as one)

The bootloader is written in C and uses parts of the GNU-EFI UEFI toolchain, with Clang.
The kernel is written in Rust loosely following the "Writing an OS in Rust" (https://os.phil-opp.com/)
tutorial as a starting point. The original C "kernel" can still be found in the src/kernel/core directory.

# Build instructions:

## Ubuntu 18.XX:

This is for a clean install with no build software or anything.

First, clone this repository:
`git clone https://github.com/ThatCodingGuy86/MimosaOS`

Then, CD into the created directory:
`cd MimosaOS`

Then, to make the build script executable:
`chmod u+x build.sh`

Then, to make the toolchain script executable:
`chmod u+x toolchain.sh`

Then, update your packages:
`sudo apt update`

Then, install Make:
`sudo apt install make`

Then, install build-essential:
`sudo apt install build-essential`

Then, install mtools:
`sudo apt-get install mtools`

Then, execute the toolchain script (This will take a while!):
`./toolchain.sh`

Then, execute the build script:
`./build.sh`

This will produce a `fat.img` file that you can use your preferred tool to put on a storage device.

# Bug Reports:

Simply open an issue with the label "bug", add a title summarizing the bug,
and add a description with both your hardware and a full description of the scenario
to trigger the bug.
