#include "../headers/system.h"
#include "../headers/multiboot.h"

#define BLOCK_SIZE 4096
#define TOTAL_MEMORY 0x1000000
#define TOTAL_BLOCKS (TOTAL_MEMORY / BLOCK_SIZE)

unsigned char bitmap[TOTAL_BLOCKS / 8];  // Bitmap array
// Set a bit in the bitmap
void set_bit(unsigned int bit) {
    bitmap[bit / 8] |= (1 << (bit % 8));
}

void clear_bit(unsigned int bit) {
    bitmap[bit / 8] &= ~(1 << (bit % 8));
}

// Check if a bit is set
unsigned int test_bit(unsigned int bit) {
    return bitmap[bit / 8] & (1 << (bit % 8));
}

// Find the first free block
unsigned int first_free() {
    unsigned int i, j;
    for (i = 0; i < TOTAL_BLOCKS / 8; i++) {
        if (bitmap[i] != 0xFF) {
            for (j = 0; j < 8; j++) {
                if (!(bitmap[i] & (1 << j))) {
                    return i * 8 + j;
                }
            }
        }
    }
    return -1;
}

// allocate a block
void alloc_block(unsigned int block) {
    set_bit(block);
}

// free a block
void free_block(unsigned int block) {
    clear_bit(block);
}

// initialize the bitmap
void init_bitmap(multiboot_info_t *mbt) {
    char buffer[256];
    unsigned int i;

    // Initialize bitmap to all blocks as used
    for (i = 0; i < TOTAL_BLOCKS; i++) {
        alloc_block(i);
    }
    puts("Initialized all blocks as used.\n");

    // Process the memory map
    memory_map_t *mmap = (memory_map_t *) mbt->mmap_addr;
    while ((unsigned int) mmap < mbt->mmap_addr + mbt->mmap_length) {
        unsigned int addr = mmap->base_addr_low;
        unsigned int num = mmap->length_low / BLOCK_SIZE;

        itoa(addr, buffer, 16);
        puts("Memory segment start: 0x");
        puts(buffer);
        puts(", length: ");
        itoa(mmap->length_low, buffer, 10);
        puts(buffer);
        puts(" blocks, type: ");
        itoa(mmap->type, buffer, 10);
        puts(buffer);
        puts("\n");

        if (mmap->type == 1) { // Available memory
            for (i = 0; i < num; i++) {
                free_block(addr / BLOCK_SIZE + i);
            }
        }

        mmap = (memory_map_t *) ((unsigned int) mmap + mmap->size + sizeof(unsigned int));
    }
    puts("Processed memory map and updated bitmap.\n");
}
