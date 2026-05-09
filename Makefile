OBJECTS = loader.o kmain.o io.o serial.o gdt.o gdt_asm.o \
          idt.o interrupt.o pic.o keyboard.o interrupt_handler_c.o

CC = gcc

CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin \
-fno-stack-protector -nostartfiles -nodefaultlibs \
-Wall -Wextra -Werror -c

LDFLAGS = -T link.ld -melf_i386

AS = nasm
ASFLAGS = -f elf

all: os.iso

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	grub-mkrescue -o os.iso iso

run:
	qemu-system-i386 -cdrom os.iso

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

io.o: io.s
	nasm -f elf io.s -o io.o

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o *.elf *.iso
