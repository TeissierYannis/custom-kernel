#include "drivers/vga.h"
#include "drivers/keyboard.h"

void k_main() {
	clear_screen();
	kprint("Hello, World from VGA driver!\n");
	init_keyboard();
}
