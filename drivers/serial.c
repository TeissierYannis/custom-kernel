#include "serial.h"
#include "ports.h"

#define PORT 0x3f8  // COM1

void init_serial() {
    outportb(PORT + 1, 0x00);
    outportb(PORT + 3, 0x80);
    outportb(PORT + 0, 0x03);
    outportb(PORT + 1, 0x00);
    outportb(PORT + 3, 0x03);
    outportb(PORT + 2, 0xC7);
    outportb(PORT + 4, 0x0B);
}

int is_transmit_empty() {
    return inportb(PORT + 5) & 0x20;
}

void write_serial(char a) {
    while (is_transmit_empty() == 0);
    outportb(PORT,a);
}
