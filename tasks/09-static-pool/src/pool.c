#include "pool.h"

#include <stdint.h>

_Static_assert(POOL_BLOCK_SIZE % _Alignof(max_align_t) == 0,
               "POOL_BLOCK_SIZE must be a multiple of the max alignment");

static _Alignas(max_align_t) unsigned char pool_mem[POOL_BLOCK_COUNT][POOL_BLOCK_SIZE];
static bool in_use[POOL_BLOCK_COUNT];

void pool_init(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        in_use[i] = false;
    }
}

void *pool_alloc(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        if (!in_use[i]) {
            in_use[i] = true;
            return pool_mem[i];
        }
    }
    return NULL;
}
/*to return a free there need to be most careful!*/
bool pool_free(void *block)
{
    if (block == NULL) {
        return false;
    }

    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        if (block == (void *)pool_mem[i]) {
            if (!in_use[i]) {
                return false; /* double free */
            }
            in_use[i] = false;
            return true;
        }
    }
    return false;
}

size_t pool_free_count(void)
{
    size_t n = 0;
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        if (!in_use[i]) {
            n++;
        }
    }
    return n;
}
