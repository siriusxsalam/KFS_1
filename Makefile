
CC      = i686-elf-gcc
AS      = nasm
LD      = i686-elf-gcc
GRUB    = grub-mkrescue
QEMU    = qemu-system-i386

CFLAGS  = -m32 -ffreestanding -fno-stack-protector -fno-pic \
          -fno-builtin -O2 -Wall -Wextra -Iinclude

LDFLAGS = -T linker.ld -ffreestanding -O2 -nostdlib -nodefaultlibs \
          -Wl,--build-id=none

BUILD   = build
ISO_DIR = iso
ISO     = $(BUILD)/kfs_1.iso
KERNEL  = $(BUILD)/kernel.elf

all: $(KERNEL)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/boot.o: src/boot/boot.asm | $(BUILD)
	$(AS) -f elf32 $< -o $@

$(BUILD)/kernel.o: src/kernel/kernel.c include/kernel.h include/screen.h include/keyboard.h include/printf.h include/types.h | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/screen.o: src/kernel/screen.c include/screen.h include/types.h | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/keyboard.o: src/kernel/keyboard.c include/keyboard.h include/types.h | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/lib.o: src/kernel/lib.c include/kernel.h include/types.h | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/printf.o: src/kernel/printf.c include/printf.h include/screen.h include/types.h | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL): $(BUILD)/boot.o $(BUILD)/kernel.o $(BUILD)/screen.o $(BUILD)/keyboard.o $(BUILD)/lib.o $(BUILD)/printf.o linker.ld
	$(LD) $(LDFLAGS) -o $@ \
		$(BUILD)/boot.o \
		$(BUILD)/kernel.o \
		$(BUILD)/screen.o \
		$(BUILD)/keyboard.o \
		$(BUILD)/lib.o \
		$(BUILD)/printf.o \
		-lgcc

iso: $(KERNEL)
	mkdir -p $(ISO_DIR)/boot
	cp $(KERNEL) $(ISO_DIR)/boot/kernel.elf
	$(GRUB) -o $(ISO) $(ISO_DIR)

run: iso
	$(QEMU) -cdrom $(ISO) -m 128M

clean:
	rm -rf $(BUILD)
	rm -f $(ISO_DIR)/boot/kernel.elf

.PHONY: all iso run clean
