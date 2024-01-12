CC = gcc
LD = ld
CFLAGS = -m32 -ffreestanding -fno-PIC -no-pie -I.
LDFLAGS = -m elf_i386
ASM = nasm
ASMFLAGS = -f elf32

all: myos.iso

myos.iso: kernel.bin
	cp kernel.bin isodir/boot
	cp grub.cfg isodir/boot/grub
	grub-mkrescue -o ./output/myos.iso isodir

kernel.bin: boot.o kernel.o vga.o keyboard.o ports.o idt_load.o linker.ld keyboard_handler.o
	$(LD) $(LDFLAGS) -T linker.ld -o kernel.bin boot.o idt_load.o kernel.o vga.o keyboard.o ports.o keyboard_handler.o

boot.o: boot.asm
	$(ASM) $(ASMFLAGS) boot.asm -o boot.o

idt_load.o: idt_load.asm
	$(ASM) $(ASMFLAGS) idt_load.asm -o idt_load.o

keyboard_handler.o: keyboard_handler.asm
	$(ASM) $(ASMFLAGS) keyboard_handler.asm -o keyboard_handler.o

kernel.o: kernel.c drivers/vga.h drivers/keyboard.h
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

vga.o: drivers/vga.c drivers/vga.h
	$(CC) $(CFLAGS) -c drivers/vga.c -o vga.o

keyboard.o: drivers/keyboard.c drivers/keyboard.h
	$(CC) $(CFLAGS) -c drivers/keyboard.c -o keyboard.o

ports.o: cpu/ports.c cpu/ports.h
	$(CC) $(CFLAGS) -c cpu/ports.c -o ports.o

clean:
	rm -rf *.bin *.o ./isodir/boot/kernel.bin ./output/myos.iso */*.o

.PHONY: all clean
