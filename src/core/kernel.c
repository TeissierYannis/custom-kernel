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

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        puts("Invalid magic number");
        return;
    }

    // Initialize GDT, IDT, ISRs, and IRQs
    puts("Loading GDT...\n");
    gdt_install();
    puts("Done.\n");
    puts("Loading IDT...\n");
    idt_install();
    puts("Done.\n");
    puts("Loading ISRS...\n");
    isrs_install();
    puts("Done.\n");
    puts("Loading IRQ...\n");
    irq_install();
    puts("Done.\n");

    puts("Loading video...\n");
    init_video();
    puts("Done.\n");
    puts("Loading timer...\n");
    timer_install();
    puts("Done.\n");
    puts("Loading keyboard...\n");
    keyboard_install();
    puts("Done.\n");

    // Initialize physical memory management
    if (CHECK_FLAG(mbi->flags, 6)) {
        puts("Loading bitmap...\n");
        init_bitmap(mbi);
        puts("Done.\n");
    }

    // Initialize paging
    puts("Loading paging...\n");
    setup_paging();
    puts("Done.\n");

    puts("Loading memory heap...\n");
    init_heap();
    puts("Done.\n");

    // Initialize filesystem and disk
    puts("Initializing filesystem...\n");
    fs_init();
    puts("Done.\n");

    puts("Initializing disk...\n");
    disk_initialize();
    puts("Done.\n");

    // Create a test file
    puts("Creating test file...\n");
    int file = fs_create("testfile");
    if (file == -1) {
        puts("Failed to create file.\n");
    } else {
        puts("Done.\n");
    }

    // Write some data to the file
    puts("Writing data to file...\n");
    const char* data = "Hello, World!";
    int bytes_written = fs_write(file, data, strlen(data));
    if (bytes_written == -1) {
        puts("Failed to write to file.\n");
    } else {
        puts("Done.\n");
    }

    // Read the data from the file
    puts("Reading data from file...\n");
    char buffer[100];
    int bytes_read = fs_read(file, buffer, sizeof(buffer));
    if (bytes_read == -1) {
        puts("Failed to read from file.\n");
    } else {
        // Null-terminate the buffer and print it
        buffer[bytes_read] = '\0';
        puts("Data read from file: ");
        puts(buffer);
        puts("\n");
    }

    // Close the file
    fs_close(file);

    puts("test");

    __asm__ __volatile__ ("sti");

    for (;;);
}