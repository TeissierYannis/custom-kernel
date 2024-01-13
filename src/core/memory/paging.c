#include "../headers/system.h"

typedef struct {
    unsigned int present    : 1;
    unsigned int rw         : 1;
    unsigned int user       : 1;
    unsigned int accessed   : 1;
    unsigned int dirty      : 1;
    unsigned int unused     : 7;
    unsigned int frame      : 20;
} page_entry_t;

typedef struct {
    page_entry_t entries[1024];
} page_table_t;

// Assuming page_table_t* is 4KB in size
page_table_t *kernel_directory[1024];

// Function to convert block number to physical address
unsigned int block_to_addr(unsigned int block) {
    return block * BLOCK_SIZE;
}

unsigned int virtual_to_physical(unsigned int virtual_addr) {
    // This is a simplified example. In a real scenario, you would
    // need to navigate the page directory and table to find the physical address.
    // This might involve handling page table entries, different page sizes, etc.

    // For now, we're assuming identity mapping.
    return virtual_addr;
}

void map_vga_buffer(page_table_t *table) {
    // Map the frame of the VGA buffer to the same virtual address (identity mapping)
    table->entries[VGA_BUFFER_FRAME].frame = VGA_BUFFER_FRAME;
    table->entries[VGA_BUFFER_FRAME].present = 1;
    table->entries[VGA_BUFFER_FRAME].rw = 1; // Read/Write
}

// Function to set up paging
void setup_paging() {
    unsigned int dir_block = first_free(); // Find a free block for the directory
    if (dir_block == -1) {
        // Handle error: no free block available
        return;
    }
    alloc_block(dir_block); // Allocate the block

    // Ensure that the page table containing the VGA buffer is mapped
    unsigned int vga_table_index = VGA_BUFFER_FRAME / 1024;
    if (kernel_directory[vga_table_index] == NULL) {
        // Allocate and set up the table that will contain the VGA buffer mapping
        // ... [allocate and set up the table as in your existing code]
    }
    map_vga_buffer(kernel_directory[vga_table_index]);

    // Initialize each page table within the directory
    for (int i = 0; i < 1024; i++) {
        unsigned int table_block = first_free();
        if (table_block == -1) {
            // Handle error: no free block available
            return;
        }
        alloc_block(table_block);

        page_table_t *table = (page_table_t *)block_to_addr(table_block);
        kernel_directory[i] = table; // Correct assignment

        for (int j = 0; j < 1024; j++) {
            table->entries[j].frame = i * 1024 + j;
            table->entries[j].present = 1;
            table->entries[j].rw = 1; // Read/Write enabled
        }
    }

    // Load the page directory address into CR3 and enable paging
    unsigned int cr0;
    unsigned int page_directory_addr = virtual_to_physical((unsigned int)kernel_directory);
    __asm__ volatile("mov %0, %%cr3" : : "r"(page_directory_addr));
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging
    __asm__ volatile("mov %0, %%cr0" : : "r"(cr0));

}
