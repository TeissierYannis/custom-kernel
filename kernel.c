#include "drivers/vga.h"
#include "drivers/keyboard.h"
#include "drivers/serial.h"
#include "cpu/ports.h"
#include "cpu/idt.h"

/* This function will be called from your assembly code */
extern void idt_load();

/* Initialize all the necessary components */
void init() {
	init_serial();       // Initialize serial port for debugging
	init_idt();          // Initialize the Interrupt Descriptor Table
	init_keyboard();     // Initialize the keyboard
	clear_screen();      // Clear the VGA screen
}

/* Kernel's main function */
void kernel_main() {
	init();  // Initialize components

	/* Now you can start using the components */
	kprint("Hello, World from VGA!\n");
	write_serial("Hello, Serial Port!\n");
}

/* You will need to link `idt_load` from your assembly code */
