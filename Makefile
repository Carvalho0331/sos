# ============================================================
# SOS — Simple Operating System
# Makefile
# ============================================================

# Directories
ASM_DIR = asm
SRC_DIR = src
INC_DIR = include

# ──────────────────────────────────────────────────────────
# Object files
# ──────────────────────────────────────────────────────────
OBJECTS = \
    $(ASM_DIR)/loader.o       \
    $(ASM_DIR)/io.o           \
    $(ASM_DIR)/gdt_asm.o      \
    $(ASM_DIR)/interrupt.o    \
    $(ASM_DIR)/syscall_stub.o \
    $(SRC_DIR)/kmain.o        \
    $(SRC_DIR)/vga.o          \
    $(SRC_DIR)/gdt.o          \
    $(SRC_DIR)/idt.o          \
    $(SRC_DIR)/interrupt_handler_c.o \
    $(SRC_DIR)/keyboard.o     \
    $(SRC_DIR)/pic.o          \
    $(SRC_DIR)/serial.o       \
    $(SRC_DIR)/pit.o          \
    $(SRC_DIR)/mm.o           \
    $(SRC_DIR)/paging.o       \
    $(SRC_DIR)/syscall.o      \
    $(SRC_DIR)/shell.o        \
    $(SRC_DIR)/task.o         \
    $(SRC_DIR)/gui.o

# ──────────────────────────────────────────────────────────
# Toolchain
# ──────────────────────────────────────────────────────────
CC = gcc
AS = nasm
LD = ld

# Compiler flags — freestanding 32-bit kernel
CFLAGS = -m32 -ffreestanding -nostdlib -nostdinc -fno-builtin \
         -fno-stack-protector -nostartfiles -nodefaultlibs     \
         -Wall -Wextra -Werror -c -I$(INC_DIR)

# Assembler flags
ASFLAGS = -f elf

# Linker flags
LDFLAGS = -T link.ld -melf_i386

# ──────────────────────────────────────────────────────────
# Targets
# ──────────────────────────────────────────────────────────
all: os.iso

# Link kernel ELF
kernel.elf: $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

# Build bootable ISO
os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	grub-mkrescue -o os.iso iso

# Run in QEMU (serial output to stdio)
run: os.iso
	qemu-system-i386 -cdrom os.iso -serial stdio

# Run without GUI window (headless, serial only)
run-headless: os.iso
	qemu-system-i386 -cdrom os.iso -nographic -serial stdio

# ──────────────────────────────────────────────────────────
# Compilation rules
# ──────────────────────────────────────────────────────────

# Assembly files
$(ASM_DIR)/%.o: $(ASM_DIR)/%.s
	$(AS) $(ASFLAGS) $< -o $@

# C files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

# ──────────────────────────────────────────────────────────
# Clean
# ──────────────────────────────────────────────────────────
clean:
	rm -f $(ASM_DIR)/*.o $(SRC_DIR)/*.o kernel.elf os.iso iso/boot/kernel.elf

.PHONY: all run run-headless clean
