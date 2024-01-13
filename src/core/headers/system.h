#ifndef __SYSTEM_H
#define __SYSTEM_H
#define NULL ((void*)0)

typedef unsigned long size_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;


/* This defines what the stack looks like after an ISR was running */
struct regs
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */
};


/* main.C */
extern void *memcpy(void *dest, const void *src, size_t count);
extern void *memset(void *dest, char val, size_t count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);
extern size_t strlen(const char *str);
extern unsigned char inportb (unsigned short _port);
extern void outportb (unsigned short _port, unsigned char _data);

/* screen.c */
extern void cls();
extern void putch(unsigned char c);
extern void puts(unsigned char *str);
extern void settextcolor(unsigned char forecolor, unsigned char backcolor);
extern void init_video();

/* gdt.c */
extern void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
extern void gdt_install();

/* idt.c */
extern void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
extern void idt_install();

/* isrs.c */
extern void isrs_install();
extern void fault_handler(struct regs *r);

/* irq.c */
extern void irq_install_handler(int irq, void (*handler)(struct regs *r));
extern void irq_uninstall_handler(int irq);
extern void irq_remap(void);
extern void irq_install();
extern void irq_handler(struct regs *r);

/* timer.c */
extern void timer_wait(int ticks);
extern void timer_phase(int hz);
extern void timer_handler(struct regs *r);
extern void timer_install();

/* keyboard.c */
extern void keyboard_handler(struct regs *r);
extern void keyboard_install();

/* phy_memory.c */
extern void set_bit(unsigned int bit);
extern void clear_bit(unsigned int bit);
extern unsigned int test_bit(unsigned int bit);
extern unsigned int first_free();
extern void alloc_block(unsigned int block);
extern void free_block(unsigned int block);

#endif