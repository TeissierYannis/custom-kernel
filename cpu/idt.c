#include "idt.h"
#include "../drivers/keyboard.h"

/* Forward declaration of the keyboard interrupt handler. */
extern void keyboard_interrupt_handler();

/* Declare an array of IDT entries */
idt_entry_t idt[IDT_SIZE];
idt_ptr_t idt_ptr;

/* Function to set an entry in the IDT */
void set_idt_gate(int n, uint32_t handler) {
    write_serial("Setting IDT gate...\n");
    idt[n].base_lo = handler & 0xFFFF;
    idt[n].base_hi = (handler >> 16) & 0xFFFF;
    idt[n].sel = 0x08;  // Segment selector set to kernel code segment
    idt[n].always0 = 0;
    idt[n].flags = 0x8E;  // Present, ring 0, '32-bit interrupt gate'
    write_serial("IDT gate set!\n");
}

/* Function to load the IDT */
extern void idt_load();  // Implemented in assembly

void init_idt() {
    write_serial("Initializing IDT...\n");
    idt_ptr.limit = sizeof(idt_entry_t) * IDT_SIZE - 1;
    idt_ptr.base = (uint32_t)&idt;
    write_serial("IDT pointer initialized!\n");

    // Set up IDT entries here
    write_serial("Setting up IDT entries...\n");
    set_idt_gate(0x21, (uint32_t)keyboard_handler);
    set_idt_gate(1, (uint32_t)keyboard_interrupt_handler);
    write_serial("IDT entries set!\n");

    write_serial("Loading IDT...\n");
    idt_load();  // Load the IDT
    write_serial("IDT loaded!\n");
}
