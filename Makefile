CC = gcc
LD = ld
CFLAGS = -m32 -ffreestanding -fno-PIC -no-pie -I.
LDFLAGS = -m elf_i386
ASM = nasm
ASMFLAGS = -f elf32

all: pre-build myos.iso

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

./build/kernel.bin: ./build/start.o ./build/kernel.o ./build/screen.o ./build/gdt_c.o ./build/gdt.o ./build/idt_c.o ./build/idt.o ./build/isrs_c.o ./build/isrs.o ./build/irq_c.o ./build/irq.o ./build/timer.o ./build/keyboard.o ./build/phys_memory.o ./build/paging.o ./build/page_alloc.o ./build/heap.o ./build/fs.o ./build/disk.o ./build/util.o ./build/processes.o
	$(LD) $(LDFLAGS) -T ./src/linker/link.ld -o ./build/kernel.bin ./build/kernel.o ./build/screen.o ./build/gdt_c.o ./build/idt_c.o ./build/irq_c.o ./build/isrs_c.o ./build/keyboard.o ./build/start.o ./build/gdt.o ./build/idt.o ./build/isrs.o ./build/irq.o ./build/timer.o ./build/phys_memory.o ./build/paging.o ./build/page_alloc.o ./build/heap.o ./build/fs.o ./build/disk.o ./build/util.o ./build/processes.o
# C files
./build/kernel.o: ./src/core/kernel.c
	$(CC) $(CFLAGS) -c src/core/kernel.c -o ./build/kernel.o

./build/screen.o: ./src/core/drivers/screen.c
	$(CC) $(CFLAGS) -c src/core/drivers/screen.c -o ./build/screen.o

./build/keyboard.o: ./src/core/drivers/keyboard.c
	$(CC) $(CFLAGS) -c src/core/drivers/keyboard.c -o ./build/keyboard.o

./build/disk.o: ./src/core/drivers/disk.c
	$(CC) $(CFLAGS) -c src/core/drivers/disk.c -o ./build/disk.o

./build/fs.o: ./src/core/filesystem/fs.c
	$(CC) $(CFLAGS) -c src/core/filesystem/fs.c -o ./build/fs.o

./build/phys_memory.o: ./src/core/memory/phys_memory.c
	$(CC) $(CFLAGS) -c src/core/memory/phys_memory.c -o ./build/phys_memory.o

./build/paging.o: ./src/core/memory/paging.c
	$(CC) $(CFLAGS) -c src/core/memory/paging.c -o ./build/paging.o

./build/page_alloc.o: ./src/core/memory/page_alloc.c
	$(CC) $(CFLAGS) -c src/core/memory/page_alloc.c -o ./build/page_alloc.o

./build/heap.o: ./src/core/memory/heap.c
	$(CC) $(CFLAGS) -c src/core/memory/heap.c -o ./build/heap.o

./build/processes.o: ./src/core/processes/processes.c
	$(CC) $(CFLAGS) -c src/core/processes/processes.c -o ./build/processes.o

./build/gdt_c.o: ./src/core/cpu/gdt.c
	$(CC) $(CFLAGS) -c src/core/cpu/gdt.c -o ./build/gdt_c.o

./build/idt_c.o: ./src/core/cpu/idt.c
	$(CC) $(CFLAGS) -c src/core/cpu/idt.c -o ./build/idt_c.o

./build/isrs_c.o: ./src/core/cpu/isrs.c
	$(CC) $(CFLAGS) -c src/core/cpu/isrs.c -o ./build/isrs_c.o

./build/irq_c.o: ./src/core/cpu/irq.c
	$(CC) $(CFLAGS) -c src/core/cpu/irq.c -o ./build/irq_c.o

./build/timer.o: ./src/core/cpu/timer.c
	$(CC) $(CFLAGS) -c src/core/cpu/timer.c -o ./build/timer.o

./build/util.o: ./src/core/util/util.c
	$(CC) $(CFLAGS) -c src/core/util/util.c -o ./build/util.o

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

.PHONY: all clean
