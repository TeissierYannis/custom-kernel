#include "keyboard.h"
#include "ports.h"
#include "idt.h" // You need to create this header and its implementation

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_IRQ 1

/* Forward declaration of the keyboard interrupt handler. */
extern void keyboard_interrupt_handler();

/* Remap the PIC */
void remap_pic() {
    /* Start initialization sequence in Command Registers */
    outportb(PIC1_COMMAND, 0x11);
    outportb(PIC2_COMMAND, 0x11);

    /* Set vector offset */
    outportb(PIC1_DATA, 0x20); // ICW2: Master PIC vector offset
    outportb(PIC2_DATA, 0x28); // ICW2: Slave PIC vector offset

    /* Configure cascading */
    outportb(PIC1_DATA, 0x04); // ICW3: tell Master PIC that there is a slave at IRQ2
    outportb(PIC2_DATA, 0x02); // ICW3: tell Slave PIC its cascade identity

    /* Set environment info */
    outportb(PIC1_DATA, 0x01); // ICW4: 8086/88 (MCS-80/85) mode
    outportb(PIC2_DATA, 0x01); // ICW4: 8086/88 (MCS-80/85) mode

    /* Mask interrupts */
    outportb(PIC1_DATA, 0x0);
    outportb(PIC2_DATA, 0x0);
}

/* Initialize the keyboard by setting up its interrupt handler */
void init_keyboard() {
    remap_pic();

    /* Set the keyboard interrupt handler in the IDT */
    set_idt_gate(KEYBOARD_IRQ, (unsigned int)keyboard_interrupt_handler);

    /* Enable keyboard interrupts (IRQ1) */
    outportb(PIC1_DATA, inportb(PIC1_DATA) & ~(1 << KEYBOARD_IRQ));
}

void keyboard_handler() {
    unsigned char scancode = inportb(KEYBOARD_DATA_PORT);

    if (scancode < 0x80) {  // Ignore key release for now
        char ascii = scancode_to_char(scancode);
        if (ascii) {
            char str[2] = {ascii, '\0'};  // Convert to string
            kprint(str);  // Print the character using your VGA driver
        }
    }
}

/* Keyboard scancode mapping to ASCII characters. */
char scancode_to_char(unsigned char scancode) {
    static char scancode_map[] = {
        0,  27, '1', '2',  '3', '4', '5', '6',  '7', '8', /* 9 */
        '9', '0', '-', '=', '\b', /* Backspace */
        '\t',         /* Tab */
        'q', 'w', 'e', 'r',   /* 19 */
        't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
        0,          /* 29   - Control */
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
        '\'', '`',   0,        /* Left shift */
        '\\', 'z', 'x', 'c', 'v', 'b', 'n',            /* 49 */
        'm', ',', '.', '/',   0,                  /* Right shift */
        '*',
        0,  /* Alt */
        ' ',  /* Space bar */
        0,  /* Caps lock */
    };

    if (scancode < sizeof(scancode_map)) {
        return scancode_map[scancode];
    }

    return 0;
}