#include "./headers/system.h"

void simple_test_process() {
    log_serial("Simple test process running.\n");
    puts("Hello from simple test process!\n");
	return;
}
// Main function
void main(unsigned long magic, unsigned long addr) {

    setup(magic, addr);

    // Enable interrupts
    __asm__ __volatile__("sti");

    simple_test_process();

    // Main loop
    for (;;) {}
}