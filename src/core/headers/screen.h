#ifndef __SCREEN_H
#define __SCREEN_H

#define VGA_BUFFER_ADDR 0xB8000
#define VGA_BUFFER_FRAME (VGA_BUFFER_ADDR / 0x1000) // Frame number for VGA buffer

extern void cls();
extern void putch(unsigned char c);
extern void puts(unsigned char *str);
extern void settextcolor(unsigned char forecolor, unsigned char backcolor);
extern void init_video();

#endif