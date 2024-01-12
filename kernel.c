#include "drivers/vga.h"
#include "drivers/serial.h"
#include "cpu/idt.h"

/* This function will be called from your assembly code */
extern void idt_load();

/* Initialize all the necessary components */
void init() {
    write_serial("Initializing components...\n");
    init_serial(); // Initialize serial port for debugging
    write_serial("Serial port initialized!\n");
    init_idt(); // Initialize the Interrupt Descriptor Table
    write_serial("IDT initialized!\n");
    write_serial("Keyboard initialized!\n");
    clear_screen(); // Clear the VGA screen
    write_serial("VGA screen cleared!\n");
}

/* Kernel's main function */
void k_main() {
    init(); // Initialize components

    /* Now you can start using the components */
    kprint("Hello, World from VGA!\n");
    write_serial("Hello, Serial Port!\n");

    //asm volatile("int $0x30");
}

/* You will need to link `idt_load` from your assembly code */
