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

kernel.bin: boot.o kernel.o vga.o keyboard.o
	$(LD) $(LDFLAGS) -T linker.ld -o kernel.bin boot.o kernel.o vga.o keyboard.o

boot.o: boot.asm
	$(ASM) $(ASMFLAGS) boot.asm -o boot.o

kernel.o: kernel.c drivers/vga.h drivers/keyboard.h
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

vga.o: drivers/vga.c drivers/vga.h
	$(CC) $(CFLAGS) -c drivers/vga.c -o vga.o

keyboard.o: drivers/keyboard.c drivers/keyboard.h
	$(CC) $(CFLAGS) -c drivers/keyboard.c -o keyboard.o

clean:
	rm -rf *.bin *.o isodir/boot/kernel.bin ./output/myos.iso */*.o

.PHONY: all clean
