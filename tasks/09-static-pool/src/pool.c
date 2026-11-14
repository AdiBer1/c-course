#include "pool.h"
 
#include <stdint.h>
 
/* Every block must start on a max_align_t boundary: the array is aligned,
 * and the block size is a multiple of the alignment, so every block is too. */
_Static_assert(POOL_BLOCK_SIZE % _Alignof(max_align_t) == 0,
               "POOL_BLOCK_SIZE must be a multiple of _Alignof(max_align_t)");
 
/* Storage: 8 blocks of 32 bytes, reserved at link time (.bss), never on the heap. */
static _Alignas(max_align_t) unsigned char pool_storage[POOL_BLOCK_COUNT][POOL_BLOCK_SIZE];
 
/* Bookkeeping: in_use[i] is true while block i is allocated. */
static bool in_use[POOL_BLOCK_COUNT];
 
/* Number of free blocks, kept in sync with in_use[]. */
static size_t free_blocks = POOL_BLOCK_COUNT;
 
void pool_init(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        in_use[i] = false;
    }
    free_blocks = POOL_BLOCK_COUNT;
}
 
void *pool_alloc(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        if (!in_use[i]) {
            in_use[i] = true;
            free_blocks--;
            return pool_storage[i];
        }
    }
    return NULL; /* pool exhausted */
}
 
bool pool_free(void *block)
{
    if (block == NULL) {
        return false;
    }
 
    /* Compare as integers: comparing pointers into different objects with < is undefined in C. */
    uintptr_t start = (uintptr_t)pool_storage;
    uintptr_t addr  = (uintptr_t)block;
    uintptr_t total = (uintptr_t)POOL_BLOCK_COUNT * POOL_BLOCK_SIZE;
 
    if (addr < start || addr - start >= total) {
        return false; /* outside the pool */
    }
 
    uintptr_t offset = addr - start;
    if (offset % POOL_BLOCK_SIZE != 0) {
        return false; /* points into the middle of a block */
    }
 
    size_t index = (size_t)(offset / POOL_BLOCK_SIZE);
    if (!in_use[index]) {
        return false; /* double free */
    }
 
    in_use[index] = false;
    free_blocks++;
    return true;
}
 
size_t pool_free_count(void)
{
    return free_blocks;
}
 
