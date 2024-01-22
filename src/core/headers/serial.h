#ifndef __SERIAL_H
#define __SERIAL_H

extern void init_serial();
extern void log_serial(char* str);
extern void log_serial_hex(unsigned int n);

#endif