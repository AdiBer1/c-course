#include "pool.h"
#include <stdalign.h>
#include <stdint.h>

// 1. Static memory pool storage with strict hardware alignment
_Alignas(max_align_t) static char pool_storage[POOL_BLOCK_COUNT * POOL_BLOCK_SIZE];

// 2. Bookkeeping array tracking whether each block is currently in use
static bool block_in_use[POOL_BLOCK_COUNT];

void pool_init(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        block_in_use[i] = false; 
    }
}

void *pool_alloc(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        if (!block_in_use[i]) { 
            block_in_use[i] = true; 
            // Return the exact memory address of the block inside the pool
            return &pool_storage[i * POOL_BLOCK_SIZE];
        }
    }
    return NULL; 
}

bool pool_free(void *block)
{
    if (block == NULL) {
        return false; 
    }

    // Calculate the byte offset from the start of the pool storage
    uintptr_t offset = (uintptr_t)block - (uintptr_t)pool_storage;

    // Check 1: Is the pointer within the pool's valid address range?
    // Check 2: Does it point precisely to the start of a block (zero remainder)?
    if (offset >= (POOL_BLOCK_COUNT * POOL_BLOCK_SIZE) || (offset % POOL_BLOCK_SIZE != 0)) {
        return false; // Foreign pointer or pointing into the middle of a block
    }

    // Calculate the block index
    size_t index = offset / POOL_BLOCK_SIZE;

    if (!block_in_use[index]) {
        return false; 
    }

    block_in_use[index] = false; 
    return true; 
}

size_t pool_free_count(void)
{
    size_t free_count = 0;
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        if (!block_in_use[i]) {
            free_count++; 
        }
    }
    return free_count;
}