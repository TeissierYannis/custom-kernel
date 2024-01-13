#include <stddef.h>

typedef struct heap_block {
    size_t size;
    struct heap_block* next;
    int free;
} heap_block_t;

#define HEAP_START 0x01000000 // Starting address of the heap
#define HEAP_SIZE  0x100000   // 1 MB heap size

heap_block_t* base = (heap_block_t*) HEAP_START;

void init_heap() {
    base->size = HEAP_SIZE - sizeof(heap_block_t);
    base->next = NULL;
    base->free = 1;
}

void* malloc(size_t size) {
    heap_block_t *current = base;
    while (current != NULL && !(current->free && current->size >= size)) {
        current = current->next;
    }

    if (!current) {
        // No suitable block found
        return NULL;
    }

    // Check if we can split the block
    if (current->size >= size + sizeof(heap_block_t)) {
        heap_block_t *new_block = (heap_block_t*)((char*)current + sizeof(heap_block_t) + size);
        new_block->size = current->size - size - sizeof(heap_block_t);
        new_block->next = current->next;
        new_block->free = 1;

        current->size = size;
        current->next = new_block;
    }

    current->free = 0;
    return (char*)current + sizeof(heap_block_t); // Return the address just after the metadata block
}

void free(void* ptr) {
    if (!ptr) {
        return;
    }
    heap_block_t *block = (heap_block_t*)((char*)ptr - sizeof(heap_block_t));
    block->free = 1;

    // Coalesce free blocks
    heap_block_t *current = base;
    while (current != NULL && current->next != NULL) {
        if (current->free && current->next->free) {
            current->size += current->next->size + sizeof(heap_block_t);
            current->next = current->next->next;
        }
        current = current->next;
    }
}
