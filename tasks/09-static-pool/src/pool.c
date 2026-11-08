#include "pool.h"

#include <stdint.h>
#include <stdalign.h>

static _Alignas(max_align_t) unsigned char pool[POOL_BLOCK_COUNT][POOL_BLOCK_SIZE];
static bool used[POOL_BLOCK_COUNT];

void pool_init(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        used[i] = false;
    }
}

void *pool_alloc(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        if (!used[i]) {
            used[i] = true;
            return pool[i];
        }
    }

    return NULL;
}

bool pool_free(void *block)
{
    if (block == NULL) {
        return false;
    }

    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        if (block == pool[i]) {
            if (!used[i]) {
                return false;
            }

            used[i] = false;
            return true;
        }
    }

    return false;
}

size_t pool_free_count(void)
{
    size_t count = 0;

    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        if (!used[i]) {
            count++;
        }
    }

    return count;
}