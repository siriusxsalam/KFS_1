# KFS_1 - Kernel From Scratch

## Description

A basic 32-bit x86 kernel developed from scratch using C and NASM Assembly.
The kernel is booted using GRUB through the Multiboot protocol and tested with QEMU.

## Technologies

- C
- NASM Assembly
- GCC Cross-Compiler (i686-elf)
- GNU Linker
- GRUB
- QEMU
- GNU Make

## Project Objectives

- Create an Assembly bootable base.
- Boot the kernel using GRUB.
- Implement a basic kernel library.
- Implement a screen interface.
- Display "42" on the screen.

## Implemented Features

- VGA text-mode screen interface.
- Cursor management and scrolling.
- Screen colors.
- Keyboard input.
- Three virtual screens with keyboard shortcuts.
- Kernel formatted output using `kprintf`.

## Architecture

- Target: i386 (32-bit x86)
- Bootloader: GRUB
- Boot protocol: Multiboot
- Languages: C and NASM

## Build

Build the kernel:

```bash
make
```

Create the bootable ISO:

```bash
make iso
```

Run the kernel using QEMU:

```bash
make run
```

Clean generated build files:

```bash
make clean
```

## Output

The final bootable ISO is available at:

`dist/kfs_1.iso`
