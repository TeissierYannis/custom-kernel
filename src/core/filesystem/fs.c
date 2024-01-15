#include "../headers/system.h"

typedef struct {
    char name[FILENAME_MAX];
    unsigned int size;
    unsigned int block;
} inode_t;

static inode_t inodes[MAX_FILES];

void fs_init() {
    memset(inodes, 0, sizeof(inodes));
}

int fs_create(const char* filename) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (inodes[i].name[0] == '\0') {
            strncpy(inodes[i].name, filename, FILENAME_MAX);
            inodes[i].block = first_free();
            alloc_block(inodes[i].block);
            return i;
        }
    }
    return -1; // No space left for new file
}

int fs_open(const char* filename) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (strcmp(inodes[i].name, filename) == 0) {
            return i;
        }
    }
    return -1; // File not found
}

int fs_read(int file, char* buf, int nbytes) {
    inode_t *inode = &inodes[file];
    if (inode->size == 0 || inode->name[0] == '\0') {
        return -1; // File is empty or does not exist
    }

    unsigned char block[BLOCK_SIZE];
    disk_read_sector(inode->block, block);

    int bytes_to_read = (nbytes < inode->size) ? nbytes : inode->size;
    memcpy(buf, block, bytes_to_read);

    return bytes_to_read; // Return the number of bytes read
}

int fs_write(int file, const char* buf, int nbytes) {
    inode_t *inode = &inodes[file];
    if (inode->name[0] == '\0') {
        return -1; // File does not exist
    }

    unsigned char block[BLOCK_SIZE];
    int bytes_to_write = (nbytes < BLOCK_SIZE) ? nbytes : BLOCK_SIZE;

    memcpy(block, buf, bytes_to_write);
    disk_write_sector(inode->block, block);

    inode->size = bytes_to_write; // Update file size
    return bytes_to_write; // Return the number of bytes written
}

void fs_close(int file) {
    // In this simple filesystem, closing a file doesn't need to do much.
    // However, in a more complex filesystem, this function might need to update metadata, flush buffers, etc.
}