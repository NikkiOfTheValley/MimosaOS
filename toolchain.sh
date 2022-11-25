
echo "This will likely take a while, so sit back and relax"

echo "Installing Rust for kernel. Do *NOT* change any settings, this will break things!"
echo "If you already have Rust installed, it may still work, but if it breaks, I'm not supporting it."

curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh

echo "Finished kernel toolchain, installing Clang for UEFI bootloader"

sudo apt install clang
sudo apt install lldb
sudo apt install lld

echo "If there were any errors, please open an issue on the Github page with the error message and a description of what enviorment you're running in."
