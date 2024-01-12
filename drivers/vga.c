#include "vga.h"

static const int VGA_WIDTH = 80;
static const int VGA_HEIGHT = 25;
unsigned short *terminal_buffer = (unsigned short*)0xB8000;
unsigned int terminal_row = 0;
unsigned int terminal_col = 0;

unsigned short vga_entry(unsigned char uc, unsigned char color) {
    return (unsigned short) uc | (unsigned short) color << 8;
}

void clear_screen() {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            const int index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', 0);
        }
    }
}

void kprint(const char *message) {
    for (int i = 0; message[i] != '\0'; i++) {
        terminal_buffer[terminal_row * VGA_WIDTH + terminal_col] = vga_entry(message[i], 15);
        terminal_col++;
        if (terminal_col >= VGA_WIDTH || message[i] == '\n') {
            terminal_col = 0;
            terminal_row++;
        }
        if (terminal_row >= VGA_HEIGHT) {
            clear_screen();
            terminal_row = 0;
        }
    }
}
