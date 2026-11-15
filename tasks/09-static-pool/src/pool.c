#include "pool.h"

#include <stdint.h>

/*
 * The storage: POOL_BLOCK_COUNT blocks of POOL_BLOCK_SIZE bytes, in static
 * memory. _Alignas makes the start of the array aligned for any type; since
 * POOL_BLOCK_SIZE (32) is a multiple of that alignment, every block is too.
 */
static _Alignas(max_align_t) unsigned char storage[POOL_BLOCK_COUNT][POOL_BLOCK_SIZE];

/* in_use[i] is true while block i is handed out. */
static bool in_use[POOL_BLOCK_COUNT];

/* How many blocks are free right now. */
static size_t free_blocks;

_Static_assert(POOL_BLOCK_SIZE % _Alignof(max_align_t) == 0u,
               "block size must keep every block aligned");

void pool_init(void)
{
    for (size_t i = 0u; i < POOL_BLOCK_COUNT; i++) {
        in_use[i] = false;
    }
    free_blocks = POOL_BLOCK_COUNT;
}

void *pool_alloc(void)
{
    for (size_t i = 0u; i < POOL_BLOCK_COUNT; i++) {
        if (!in_use[i]) {
            in_use[i] = true;
            free_blocks--;
            return storage[i];
        }
    }
    return NULL; /* pool exhausted */
}

bool pool_free(void *block)
{
    if (block == NULL) {
        return false;
    }

    /* Compare addresses as integers: comparing pointers into different
       objects with < or > is undefined behaviour in C. */
    uintptr_t start = (uintptr_t)&storage[0][0];
    uintptr_t addr = (uintptr_t)block;
    uintptr_t total = (uintptr_t)POOL_BLOCK_COUNT * POOL_BLOCK_SIZE;

    if (addr < start || addr >= start + total) {
        return false; /* outside the pool */
    }
    uintptr_t offset = addr - start;
    if (offset % POOL_BLOCK_SIZE != 0u) {
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
