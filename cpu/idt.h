#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IDT_SIZE 256

/* Structure of an IDT entry */
struct idt_entry_struct {
    uint16_t base_lo;  // The lower 16 bits of the address to jump to
    uint16_t sel;      // Kernel segment selector
    uint8_t  always0;  // Set to zero
    uint8_t  flags;    // Flags
    uint16_t base_hi;  // The upper 16 bits of the address to jump to
} __attribute__((packed));

typedef struct idt_entry_struct idt_entry_t;

/* Structure to load the IDT */
struct idt_ptr_struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

typedef struct idt_ptr_struct idt_ptr_t;

idt_ptr_t idt_ptr;

void init_idt();
void set_idt_gate(int n, uint32_t handler);

#endif
