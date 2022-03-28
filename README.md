# Build instructions:

## Ubuntu 18.XX:

This is for a clean install with no build software or anything.

First, clone this repository:
`git clone https://github.com/ThatCodingGuy86/UnnamedOS-V2`

Then, CD into the created directory:
`cd UnnamedOS-V2`

Then, to make the build script executable:
`chmod u+x build.sh`

Then, update your packages:
`sudo apt update`

Then, install Make:
`sudo apt install make`

Then, install build-essential:
`sudo apt install build-essential`

Then, install mtools:
`sudo apt-get install mtools`

Then, install this weird version of GCC:
`sudo apt install gcc-mingw-w64-x86-64`

Then, execute the build script:
`./build.sh`

This will produce a `fat.img` file that you can use your preferred tool to put on a storage device. (I use Rufus to do this, I'm not sure if other tools would work.)
