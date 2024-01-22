#include "./headers/system.h"

void welcome() {
    puts("==================================================\n");
    puts("=                                                =\n");
    puts("=              Welcome to 64-bit OS!             =\n");
    puts("=                                                =\n");
    puts("==================================================\n");
}

int setup(unsigned long magic, unsigned long addr) {
    init_serial();

    multiboot_info_t *mbi;

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        log_serial("Invalid magic number");
        return;
    }

    // Initialize system components
    gdt_install();
    idt_install();
    isrs_install();
    irq_install();
    init_video();
    timer_install();
    keyboard_install();



    // Welcome message
    welcome();
}