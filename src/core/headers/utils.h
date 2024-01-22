#ifndef __UTILS_H
#define __UTILS_H

#define NULL ((void*)0)

typedef unsigned long size_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

#ifdef memcpy
    extern void *memcpy(void *dest, const void *src, size_t count);
#endif
#ifdef memset
extern void *memset(void *dest, char val, size_t count);
#endif
#ifdef memsetw
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);
#endif
extern int memcmp(const void *s1, const void *s2, size_t n);
extern size_t strlen(const char *str);
extern unsigned char inportb (unsigned short _port);
extern void outportb (unsigned short _port, unsigned char _data);

#endif