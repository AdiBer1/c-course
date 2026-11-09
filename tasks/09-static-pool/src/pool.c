#include "pool.h"

#include <stdint.h>
#include <stddef.h>

static union {
    max_align_t align;
    uint8_t bytes[POOL_BLOCK_SIZE];
} blocks[POOL_BLOCK_COUNT];

static bool free_map[POOL_BLOCK_COUNT];

void pool_init(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++)
        free_map[i] = true;
}

void *pool_alloc(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        if (free_map[i]) {
            free_map[i] = false;
            return blocks[i].bytes;
        }
    }
    return NULL;
}

bool pool_free(void *block)
{
    if (!block) return false;
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        if ((void *)blocks[i].bytes == block) {
            if (!free_map[i]) {
                free_map[i] = true;
                return true;
            }
            return false; /* double free */
        }
    }
    return false; /* not a pool block */
}

size_t pool_free_count(void)
{
    size_t n = 0;
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++)
        if (free_map[i]) n++;
    return n;
}
