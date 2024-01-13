#include "./headers/system.h"
#include "./headers/multiboot.h"

#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002
#define CHECK_FLAG(flags,bit)	((flags) & (1 << (bit)))

void *memcpy(void *dest, const void *src, size_t count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
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
void display_memory_map(multiboot_info_t *mbi) {
    if (mbi->flags & (1 << 6)) {
        memory_map_t *mmap = (memory_map_t *)(unsigned long)mbi->mmap_addr;
        char buffer[256];
        char num_buffer[64]; // Buffer for numbers

        while ((unsigned long)mmap < mbi->mmap_addr + mbi->mmap_length) {
            buffer[0] = '\0'; // Clear buffer

            strcat(buffer, "Memory segment start: 0x");
            itoa(mmap->base_addr_high, num_buffer, 16);
            strcat(buffer, num_buffer);
            itoa(mmap->base_addr_low, num_buffer, 16);
            strcat(buffer, num_buffer);
            strcat(buffer, ", length: 0x");
            itoa(mmap->length_high, num_buffer, 16);
            strcat(buffer, num_buffer);
            itoa(mmap->length_low, num_buffer, 16);
            strcat(buffer, num_buffer);
            strcat(buffer, ", type: ");
            itoa(mmap->type, num_buffer, 10);
            strcat(buffer, num_buffer);
            strcat(buffer, "\n");

            puts(buffer);

            mmap = (memory_map_t *)((unsigned long)mmap + mmap->size + sizeof(mmap->size));
        }
    } else {
        puts("Memory map not available.\n");
    }
}


extern void init_bitmap(multiboot_info_t *mbt);

void main(unsigned long magic, unsigned long addr)
{
    multiboot_info_t *mbi;
    char buffer[256];

    gdt_install();
    idt_install();
    isrs_install();
    irq_install();
    init_video();
    timer_install();
    keyboard_install();

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        puts ("Invalid magic number");
        return;
    }
    /* Set MBI to the address of the Multiboot information structure.  */
    mbi = (multiboot_info_t *) addr;

    /* Are mem_* valid?  */
    if (CHECK_FLAG(mbi->flags, 0)) {
        itoa(mbi->mem_lower, buffer, 10);
        puts("mem_lower = ");
        puts(buffer);
        puts("KB, mem_upper = ");
        itoa(mbi->mem_upper, buffer, 10);
        puts(buffer);
        puts("KB\n");
    }

    /* Is boot_device valid?  */
    if (CHECK_FLAG(mbi->flags, 1)) {
        itoa(mbi->boot_device, buffer, 16);  // Convert to hex
        puts("boot_device = 0x");
        puts(buffer);
        puts("\n");
    }

    /* Are mods_* valid?  */
    // Modules
    if (CHECK_FLAG(mbi->flags, 3)) {
        module_t *mod = (module_t *)mbi->mods_addr;
        for (int i = 0; i < mbi->mods_count; i++, mod++) {
            puts("Module start: ");
            itoa(mod->mod_start, buffer, 16);
            puts(buffer);
            puts(", end: ");
            itoa(mod->mod_end, buffer, 16);
            puts(buffer);
            puts(", string: ");
            puts("\n");
        }
    }

    /* Bits 4 and 5 are mutually exclusive!  */
    if (CHECK_FLAG (mbi->flags, 4) && CHECK_FLAG (mbi->flags, 5))
    {
        puts ("Both bits 4 and 5 are set.\n");
        return;
    }

    /* Is the symbol table of a.out valid?  */
    if (CHECK_FLAG(mbi->flags, 4)) {
        aout_symbol_table_t *aout_sym = &(mbi->u.aout_sym);
        char buffer[256];

        strcpy(buffer, "aout_symbol_table: tabsize = 0x");
        itoa(aout_sym->tabsize, buffer + strlen(buffer), 16);
        strcat(buffer, ", strsize = 0x");
        itoa(aout_sym->strsize, buffer + strlen(buffer), 16);
        strcat(buffer, ", addr = 0x");
        itoa((unsigned)aout_sym->addr, buffer + strlen(buffer), 16);
        strcat(buffer, "\n");

        puts(buffer);
    }

    /* Is the section header table of ELF valid?  */
    if (CHECK_FLAG(mbi->flags, 5)) {
        elf_section_header_table_t *elf_sec = &(mbi->u.elf_sec);
        char buffer[256];

        strcpy(buffer, "elf_sec: num = ");
        itoa(elf_sec->num, buffer + strlen(buffer), 10);
        strcat(buffer, ", size = 0x");
        itoa(elf_sec->size, buffer + strlen(buffer), 16);
        strcat(buffer, ", addr = 0x");
        itoa((unsigned)elf_sec->addr, buffer + strlen(buffer), 16);
        strcat(buffer, ", shndx = 0x");
        itoa(elf_sec->shndx, buffer + strlen(buffer), 16);
        strcat(buffer, "\n");

        puts(buffer);
    }

    display_memory_map(mbi);

    init_bitmap(mbi);
	setup_paging();


    __asm__ __volatile__ ("sti");

    //    i = 10 / 0;
    //    putch(i);

    for (;;);
}