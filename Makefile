all: myos.iso

kernel.bin: boot.o kernel.o linker.ld
	ld -m elf_i386 -o kernel.bin -T linker.ld boot.o kernel.o

boot.o: boot.asm
	nasm -f elf32 boot.asm -o boot.o

kernel.o: kernel.c
	gcc -m32 -ffreestanding -c kernel.c -o kernel.o -fno-PIC -no-pie

myos.iso: kernel.bin
	cp kernel.bin isodir/boot
	cp grub.cfg isodir/boot/grub
	grub-mkrescue -o ./output/myos.iso isodir

clean:
	rm -rf *.bin *.o isodir/boot/kernel.bin ./output/myos.iso

.PHONY: all clean
