# Contribution:

To contribute, you must have your contribution in a pull request,
with an adequate summary of what is added or modified. 

You must follow the given style guide:

## Style Guide

### Folder Structure (Bootloader)

The bootloader must be contained within 1 top level directory named "bootloader",
with a "bootloader.c" in said directory. All .h files must be in a 2nd level directory named "includes",
with no other folder structure strictly required within "includes", although you can add directories
for organization purposes.

### Folder Structure (Kernel)

The kernel must be contained within 1 top level directory named "kernel", with the directory and folder 
structure described in the following ASCII diagram:

```
                            "kernel"
                               |
                    -------------------------------------------------------------------------
                "core"                                                                   "drivers"
                   |                                                                         |
            ------------------------                        --------------------------------------------------------------
        "kernel.rs"            "include"                    "[DRIVER_NAME]"                                          "lib"
                                   |                               |                                                   |
                    Extra .rs files go here         .rs files for the driver go here     This is a global library folder for all drivers
```
  
### Code Style

For the kernel, use the formatting provided by `rustfmt`.

Variable Declarations (bootloader):

For constants that are defined at compile-time (referred to as compilation constants),
you use UPPER_SNAKE_CASE. This also applies to `#define`ed constants.

For constants that are undefined at compile-time (or are defined to be an invalid value) and are
defined at runtime (referred to as runtime constants), and are only modified once,
you use lower_snake_case.

For variables that are modified more than once, (i.e "normal" variables)
you can use lowerPascalCase or UpperPascalCase, it's up to your preference.


Type Declarations (bootloader):

For type declarations of any type, you use \[NAME\]_t

For struct declarations, you use \[NAME\]_s


Functions (bootloader):

For function arguments that are directly setting some value of the same
name, (referred to as "initilization arguments") you use _\[NAME\], where NAME is in lower_snake_case.

For function arguemnts that are not directly setting some value of the same name, 
you use lowerPasalCase.

For function names, you use lower_snake_case.


File Names:

Use lower_snake_case for file and directory names.