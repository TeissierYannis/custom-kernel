#include "../headers/system.h"

static unsigned char* disk;
#define SECTOR_SIZE 512 // Assuming sector size is 512 bytes

void disk_initialize(unsigned int disk_size) {
    // Allocate memory for the disk
    disk = (unsigned char*)malloc(SECTOR_SIZE * disk_size);
    if (disk == NULL) {
        // Handle error
        return;
    }

    // Initialize disk (fill with zeros or load from a file, etc.)
    memset(disk, 0, SECTOR_SIZE * disk_size); // Initialize the disk to zero
}

void disk_read_sector(unsigned int sector, unsigned char* buffer) {
    // Read a sector from the 'disk' array
    memcpy(buffer, &disk[sector * SECTOR_SIZE], SECTOR_SIZE);
}

void disk_write_sector(unsigned int sector, const unsigned char* buffer) {
    // Write a sector to the 'disk' array
    memcpy(&disk[sector * SECTOR_SIZE], buffer, SECTOR_SIZE);
}