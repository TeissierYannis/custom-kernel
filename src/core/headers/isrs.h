#ifndef __ISRS_H
#define __ISRS_H

extern void isrs_install();
extern void fault_handler(struct regs *r);

#endif