CC = gcc
LD = ld
CFLAGS = -m32 -ffreestanding -fno-PIC -no-pie -I.
LDFLAGS = -m elf_i386
ASM = nasm
ASMFLAGS = -f elf32

all: pre-build myos.iso
test: test

pre-build:
	mkdir -p isodir/boot/grub
	mkdir -p output
	mkdir -p build

post-build:
	rm -rf build isodir

myos.iso: ./build/kernel.bin
	cp ./build/kernel.bin isodir/boot
	cp ./src/config/grub.cfg isodir/boot/grub
	mkdir -p output
	grub-mkrescue -o ./output/myos.iso isodir

# From o files

./build/kernel.bin: ./build/start.o ./build/kernel.o ./build/screen.o ./build/setup.o ./build/gdt_c.o ./build/gdt.o ./build/idt_c.o ./build/idt.o ./build/isrs_c.o ./build/isrs.o ./build/irq_c.o ./build/irq.o ./build/timer.o ./build/keyboard.o ./build/serial.o ./build/utils.o
	$(LD) $(LDFLAGS) -T ./src/linker/link.ld -o ./build/kernel.bin $^
# C files
./build/kernel.o: ./src/core/kernel.c
	$(CC) $(CFLAGS) -c src/core/kernel.c -o ./build/kernel.o

./build/screen.o: ./src/core/drivers/screen.c
	$(CC) $(CFLAGS) -c src/core/drivers/screen.c -o ./build/screen.o

./build/keyboard.o: ./src/core/drivers/keyboard.c
	$(CC) $(CFLAGS) -c src/core/drivers/keyboard.c -o ./build/keyboard.o

./build/gdt_c.o: ./src/core/cpu/gdt.c
	$(CC) $(CFLAGS) -c src/core/cpu/gdt.c -o ./build/gdt_c.o

./build/idt_c.o: ./src/core/cpu/idt.c
	$(CC) $(CFLAGS) -c src/core/cpu/idt.c -o ./build/idt_c.o

./build/setup.o: ./src/core/setup.c
	$(CC) $(CFLAGS) -c src/core/setup.c -o ./build/setup.o

./build/isrs_c.o: ./src/core/cpu/isrs.c
	$(CC) $(CFLAGS) -c src/core/cpu/isrs.c -o ./build/isrs_c.o

./build/irq_c.o: ./src/core/cpu/irq.c
	$(CC) $(CFLAGS) -c src/core/cpu/irq.c -o ./build/irq_c.o

./build/timer.o: ./src/core/cpu/timer.c
	$(CC) $(CFLAGS) -c src/core/cpu/timer.c -o ./build/timer.o

./build/serial.o: ./src/core/drivers/serial.c
	$(CC) $(CFLAGS) -c src/core/drivers/serial.c -o ./build/serial.o

./build/utils.o: ./src/core/helper/utils.c
	$(CC) $(CFLAGS) -c src/core/helper/utils.c -o ./build/utils.o

# AS
./build/start.o: ./src/asm/start.asm
	$(ASM) $(ASMFLAGS) ./src/asm/start.asm -o ./build/start.o

./build/gdt.o: ./src/asm/gdt.asm
	$(ASM) $(ASMFLAGS) ./src/asm/gdt.asm -o ./build/gdt.o

./build/idt.o: ./src/asm/idt.asm
	$(ASM) $(ASMFLAGS) ./src/asm/idt.asm -o ./build/idt.o

./build/isrs.o: ./src/asm/isrs.asm
	$(ASM) $(ASMFLAGS) ./src/asm/isrs.asm -o ./build/isrs.o

./build/irq.o: ./src/asm/irq.asm
	$(ASM) $(ASMFLAGS) ./src/asm/irq.asm -o ./build/irq.o

clean:
	rm -rf ./output ./isodir ./build

.PHONY: all clean test
