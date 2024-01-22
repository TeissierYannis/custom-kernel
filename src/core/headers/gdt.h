#ifndef __GDT_H
#define __GDT_H

extern void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
extern void gdt_install();
extern void gdt_uninstall();

#endif