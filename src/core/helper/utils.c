#include "../headers/utils.h"

void *memcpy(void *dest, const void *src, size_t count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
    const unsigned char *p1 = s1, *p2 = s2;
    while(n--)
    {
        if( *p1 != *p2 )
            return *p1 - *p2;
        p1++, p2++;
    }
    return 0;
}

void *memset(void *dest, char val, size_t count)
{
    char *temp = (char *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count)
{
    unsigned short *temp = (unsigned short *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

size_t strlen(const char *str)
{
    size_t retval;
    for(retval = 0; *str != '\0'; str++) retval++;
    return retval;
}

unsigned char inportb (unsigned short _port)
{
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outportb (unsigned short _port, unsigned char _data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

void put_hex(unsigned int value) {
    char hex_string[9]; // 8 hex digits + string terminator
    char *hex_chars = "0123456789ABCDEF";
    hex_string[8] = '\0'; // Null-terminate the string

    for (int i = 7; i >= 0; i--) {
        hex_string[i] = hex_chars[value & 0xF]; // Get the last hex digit
        value >>= 4; // Move to the next digit
    }

    puts("0x");
    puts(hex_string);
}

void itoa(unsigned int num, char *str, int base) {
    char *p = str;
    char *p1, *p2;
    unsigned long long_val = num;
    do {
        *p++ = "0123456789abcdef"[long_val % base];
    } while (long_val /= base);

    // Terminate string in buffer.
    *p = '\0';

    // Reverse the string.
    p1 = str;
    p2 = p - 1;
    while (p1 < p2) {
        char tmp = *p1;
        *p1++ = *p2;
        *p2-- = tmp;
    }
}
char* strcat(char *dest, const char *src) {
    char *dest_end = dest;

    // Move to the end of the destination string
    while (*dest_end != '\0') {
        dest_end++;
    }

    // Append each character of the source to the destination
    while (*src != '\0') {
        *dest_end = *src;
        dest_end++;
        src++;
    }

    // Null-terminate the resulting string
    *dest_end = '\0';

    return dest;
}

char* strcpy(char *dest, const char *src) {
    char *dest_orig = dest;

    // Copy each character from src to dest
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }

    // Null-terminate the destination string
    *dest = '\0';

    return dest_orig;  // Return the original pointer to dest
}

char* strstr(char* str, char* substr) {
    if (!*substr) {
        return str;
    }
    char* p1 = str, *p2 = substr;
    char* p1Adv = str;
    while (*++p2) {
        p1Adv++;
    }
    while (*p1Adv) {
        char* p1Begin = p1;
        p2 = substr;
        while (*p1 && *p2 && *p1 == *p2) {
            p1++;
            p2++;
        }
        if (!*p2) {
            return p1Begin;
        }
        p1 = p1Begin + 1;
        p1Adv++;
    }
    return NULL;
}