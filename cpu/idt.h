#ifndef IDT_H
#define IDT_H

#include <stdint.h>
/* Declare an IDT of 256 entries. Although we will only use the
*  first 32 entries in this tutorial, the rest exists as a bit
*  of a trap. If any undefined IDT entry is hit, it normally
*  will cause an "Unhandled Interrupt" exception. Any descriptor
*  for which the 'presence' bit is cleared (0) will generate an
*  "Unhandled Interrupt" exception */
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

extern void idt_load();

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);

#endif
