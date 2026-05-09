# Directories
ASM_DIR = asm
SRC_DIR = src
INC_DIR = include

# Object files (full paths)
OBJECTS = $(ASM_DIR)/loader.o \
          $(ASM_DIR)/io.o \
          $(ASM_DIR)/gdt_asm.o \
          $(ASM_DIR)/interrupt.o \
          $(SRC_DIR)/kmain.o \
          $(SRC_DIR)/gdt.o \
          $(SRC_DIR)/idt.o \
          $(SRC_DIR)/interrupt_handler_c.o \
          $(SRC_DIR)/keyboard.o \
          $(SRC_DIR)/pic.o \
          $(SRC_DIR)/serial.o 
         

# Tools
CC = gcc
AS = nasm
LD = ld

# Compiler flags for freestanding kernel
CFLAGS = -m32 -ffreestanding -nostdlib -nostdinc -fno-builtin \
         -fno-stack-protector -nostartfiles -nodefaultlibs \
         -Wall -Wextra -Werror -c -I$(INC_DIR)

# Assembler flags
ASFLAGS = -f elf

# Linker flags
LDFLAGS = -T link.ld -melf_i386

# Targets
all: os.iso

# Link kernel
kernel.elf: $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

# Build ISO image
os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	grub-mkrescue -o os.iso iso

# Run in QEMU
run: os.iso
	qemu-system-i386 -cdrom os.iso -serial stdio

# Rules for assembly files (in asm directory)
$(ASM_DIR)/%.o: $(ASM_DIR)/%.s
	$(AS) $(ASFLAGS) $< -o $@

# Rules for C files (in src directory)
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

# Clean generated files
clean:
	rm -f $(ASM_DIR)/*.o $(SRC_DIR)/*.o kernel.elf os.iso

# Phony targets
.PHONY: all run clean
