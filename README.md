## What is SnotreeOS
SnotreeOS is a simple operating system project with a fundamental kernel. It's primary goal is to boot seamlessly from an USB drive on any x86 processor-based machine, providing users with the capability to create and store files directly on the same USB drive.

## Features
- 32-bit Protected Mode
- VGA Graphics
- Interrupts
- Keyboard Input
- Serial port
- Timer
- Pseudo file system

"Pseudo file system" indicates that the OS currently supports the creation, reading, updating, and deletion of text files. However, these files are not persistent and are lost after a reboot. The implementation of a hard disk drive driver is necessary to establish a fully functional and persistent file system.

## Features yet to be added
- HDD driver
- USB drive driver

## How to run
To compile, build, and emulate the OS, you need to set up the [i686 cross compiler.](https://wiki.osdev.org/GCC_Cross-Compiler) as well as
- GRUB
- Xorriso
- GNU make 4.0 or later
- Qemu

After that, use the following commands in project directory:
```
./headers.sh
./iso.sh
./qemu.sh
```
For clearing the source direcotory run:
```
./clean.sh
```
## Mini-Manual
1. After choosing "create file" option, you can create and save the text file by pressing 'ESC' key; it will redirect you to the main screen.<br>
2. When you have opened an existing text file and want to save changes, press the left 'ALT' key.<br>
3. Press the 'F1' key for a one-row backspace.<br>

## How it looks

![snotreeos12](https://github.com/drlz0/SnotreeOS/assets/121700730/c4b2a484-93d0-4cd8-be41-a36bc9607ff1)


 
