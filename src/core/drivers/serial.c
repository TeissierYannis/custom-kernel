#include "../headers/system.h"

#define PORT 0x3f8   // COM1

void init_serial() {
   outportb(PORT + 1, 0x00);    // Disable all interrupts
   outportb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outportb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outportb(PORT + 1, 0x00);    //                  (hi byte)
   outportb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   outportb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outportb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int is_transmit_empty() {
   return inportb(PORT + 5) & 0x20;
}

void write_serial(char a) {
   while (is_transmit_empty() == 0);

   outportb(PORT,a);
}

void log_serial_hex(unsigned int n) {
    char str[9];
    itoa(n, str, 16);
    log_serial(str);
}

void log_serial_int(char* format, int value) {
    char str[12];  // Buffer to hold the string representation of the integer
    itoa(value, str, 10);

    // Replace the format specifier with the string representation of the integer
    char* p = strstr(format, "%d");
    if (p != NULL) {
        *p = '\0';  // Null-terminate the format string at the position of the format specifier
        log_serial(format);
        log_serial(str);
        log_serial(p + 2);  // Skip the format specifier and log the rest of the format string
    }
}

void log_serial_str(char* format, char* value) {
    // Replace the format specifier with the string
    char* p = strstr(format, "%s");
    if (p != NULL) {
        *p = '\0';  // Null-terminate the format string at the position of the format specifier
        log_serial(format);
        log_serial(value);
        log_serial(p + 2);  // Skip the format specifier and log the rest of the format string
    }
}

void log_serial(char* message) {
    for (int i = 0; i < strlen(message); i++) {
        write_serial(message[i]);
    }
}