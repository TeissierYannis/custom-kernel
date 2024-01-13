#include "../headers/system.h" // Include necessary headers

extern unsigned char bitmap[]; // Your bitmap array defined elsewhere

// Function to mark a page as used in the bitmap
void set_page_used(int page) {
    int byte_index = page / 8;
    int bit_offset = page % 8;
    bitmap[byte_index] |= (1 << bit_offset);
}

// Function to mark a page as free in the bitmap
void set_page_free(int page) {
    int byte_index = page / 8;
    int bit_offset = page % 8;
    bitmap[byte_index] &= ~(1 << bit_offset);
}

// Function to check if a page is free
int is_page_free(int page) {
    int byte_index = page / 8;
    int bit_offset = page % 8;
    return (bitmap[byte_index] & (1 << bit_offset)) == 0;
}

// Find a free page and return its index, -1 if none are free
int find_free_page() {
    for (int i = 0; i < TOTAL_PAGES; i++) {
        if (is_page_free(i)) {
            return i;
        }
    }
    return -1; // No free page found
}

void* allocate_page() {
    int page = find_free_page();
    if (page == -1) {
        return NULL; // No free page available
    }

    set_page_used(page);
    return (void *)(page * PAGE_SIZE); // Return the physical address of the page
}

void free_page(void* page_addr) {
    int page = (int)page_addr / PAGE_SIZE;
    set_page_free(page);
}