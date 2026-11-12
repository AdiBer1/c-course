#include "pool.h"

#include <stdint.h>

/*
 * Storage: one static 2-D array, POOL_BLOCK_COUNT blocks of POOL_BLOCK_SIZE
 * bytes each. It lives in static memory (.bss), reserved by the linker at
 * build time, and exists for the whole run of the program.
 *
 * _Alignas makes the START of the array aligned for any type. Every block
 * starts at a multiple of POOL_BLOCK_SIZE from there, so if the block size
 * is itself a multiple of the alignment, EVERY block is aligned too.
 */
_Static_assert(POOL_BLOCK_SIZE % _Alignof(max_align_t) == 0u,
               "POOL_BLOCK_SIZE must be a multiple of _Alignof(max_align_t)");

static _Alignas(max_align_t) unsigned char storage[POOL_BLOCK_COUNT][POOL_BLOCK_SIZE];

/* Bookkeeping: one "in use" flag per block. */
static bool in_use[POOL_BLOCK_COUNT];
static size_t free_count = POOL_BLOCK_COUNT;

void pool_init(void)
{
    for (size_t i = 0u; i < POOL_BLOCK_COUNT; i++) {
        in_use[i] = false;
    }
    free_count = POOL_BLOCK_COUNT;
}

void *pool_alloc(void)
{
    /* At most POOL_BLOCK_COUNT steps: bounded time. */
    for (size_t i = 0u; i < POOL_BLOCK_COUNT; i++) {
        if (!in_use[i]) {
            in_use[i] = true;
            free_count--;
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

    /*
     * Compare addresses as integers (uintptr_t): in C, `<` between pointers
     * into different objects is undefined, and `block` may point anywhere.
     */
    uintptr_t start = (uintptr_t)&storage[0][0];
    uintptr_t addr = (uintptr_t)block;
    uintptr_t total = (uintptr_t)POOL_BLOCK_COUNT * POOL_BLOCK_SIZE;

    /* Outside the pool. (addr < start is checked first so the
     * subtraction below can never wrap around.) */
    if (addr < start || addr - start >= total) {
        return false;
    }

    uintptr_t offset = addr - start;

    /* Inside the pool but not at the start of a block, e.g. p + 1. */
    if (offset % POOL_BLOCK_SIZE != 0u) {
        return false;
    }

    size_t index = (size_t)(offset / POOL_BLOCK_SIZE);

    /* Double free: the block is already free. */
    if (!in_use[index]) {
        return false;
    }

    in_use[index] = false;
    free_count++;
    return true;
}

size_t pool_free_count(void)
{
    return free_count;
}