#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stddef.h>
#include "falloc.h"

static uint64_t count_set_bits(uint64_t value) {
    uint64_t count = 0;
    while (value) {
        count += value & 1;
        value >>= 1;
    }
    return count;
}

void falloc_init(file_allocator_t* allocator, const char* filepath, uint64_t allowed_page_count) {
    allocator->fd = open(filepath, O_RDWR | O_CREAT, 0777);
    if (allocator->fd == -1) {
        printf("Error opening file\n");
        exit(0);
    }

    uint64_t sz = allowed_page_count * PAGE_SIZE + PAGE_MASK_SIZE * sizeof(uint64_t);
   
    if (ftruncate(allocator->fd, sz) == -1) {
        printf("Error truncating file\n");
        close(allocator->fd);
        exit(0);
    }

    allocator->base_addr = mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_SHARED, allocator->fd, 0);
    if (allocator->base_addr == MAP_FAILED) {
        printf("Error while mapping file\n");
        close(allocator->fd);
        exit(0);
    }
    struct stat st;
    if (fstat(allocator->fd, &st) == -1) {
        printf("Error getting file status\n");
        munmap(allocator->base_addr, sz);
        close(allocator->fd);
        exit(0);
    }
    allocator->base_addr = (void*)((char*)allocator->base_addr + PAGE_MASK_SIZE * sizeof(uint64_t));
    allocator->page_mask = (uint64_t*)((char*)allocator->base_addr - PAGE_MASK_SIZE * sizeof(uint64_t));
    allocator->allowed_page_count = allowed_page_count;

    if (st.st_size == 0) {
        allocator->curr_page_count = 0;
        memset(allocator->page_mask, 0, PAGE_MASK_SIZE * sizeof(uint64_t));
    } else {
        allocator->curr_page_count = 0;
        for (uint64_t i = 0; i < PAGE_MASK_SIZE; ++i) {
            allocator->curr_page_count += count_set_bits(allocator->page_mask[i]);
        }
    }
}



void falloc_destroy(file_allocator_t* allocator) {
    uint64_t sz = allocator->allowed_page_count * PAGE_SIZE + PAGE_MASK_SIZE * sizeof(uint64_t);
    int res = munmap(allocator->page_mask, sz);
    if (res == -1) {
        printf("Error unmapping file");
        exit(0);
    }
    int cl = close(allocator->fd);
    if (cl == -1) {
        printf("Error while closing file");
        exit(0);
    }
    
    allocator->page_mask = NULL;
    allocator->base_addr = NULL;
}

void* falloc_acquire_page(file_allocator_t* allocator) {
    if (allocator->curr_page_count >= allocator->allowed_page_count) {
        printf("NO FREE PAGES");
        return NULL;
    }
    void* addr = NULL;
    for (uint64_t i = 0; i < PAGE_MASK_SIZE; ++i) {
        if (allocator->page_mask[i] != UINT64_MAX) {
            for (uint64_t j = 0; j < 64; ++j) {
                if (!(allocator->page_mask[i] & (1ULL << j))) { 
                    allocator->page_mask[i] |= (1ULL << j);
                    ++allocator->curr_page_count;
                    uint64_t page_index = i * 64 + j;
                    addr = (void*)((char*)allocator->base_addr + page_index * PAGE_SIZE);
                    goto here;
                }
            }
        }
    }

    here:
    return addr;
}

void falloc_release_page(file_allocator_t* allocator, void** addr) {
    if (!addr || !*addr) {
        printf("Invalid address");
        return;
    }
    uint64_t page_offset = (uint64_t)((char*)*addr - (char*)allocator->base_addr);
    if (page_offset % PAGE_SIZE != 0) {
        return;
    }
    uint64_t page_index = page_offset / PAGE_SIZE;

    uint64_t mask_index = page_index / 64;
    uint64_t bit_index = page_index % 64;
    if (allocator->page_mask[mask_index] & (1ULL << bit_index)) {
        allocator->page_mask[mask_index] &= ~(1ULL << bit_index);
        --allocator->curr_page_count;
    }
    *addr = NULL;
}
