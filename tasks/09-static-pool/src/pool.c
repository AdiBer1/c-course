
#include "pool.h"

#include <stdint.h>

_Alignas(max_align_t)
static unsigned char storage[POOL_BLOCK_COUNT][POOL_BLOCK_SIZE];

static bool used[POOL_BLOCK_COUNT];

void pool_init(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++)
    {
        used[i] = false;
    }
}

void *pool_alloc(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++)
    {
        if (!used[i])
        {
            used[i] = true;
            return storage[i];
        }
    }

    return NULL;
}

bool pool_free(void *block)
{
    uintptr_t base;
    uintptr_t address;
    uintptr_t offset;
    size_t index;

    if (block == NULL)
    {
        return false;
    }

    base = (uintptr_t)&storage[0][0];
    address = (uintptr_t)block;

    if (address < base || address >= base + sizeof(storage))
    {
        return false;
    }

    offset = address - base;

    if (offset % POOL_BLOCK_SIZE != 0)
    {
        return false;
    }

    index = (size_t)(offset / POOL_BLOCK_SIZE);

    if (!used[index])
    {
        return false;
    }

    used[index] = false;
    return true;
}

size_t pool_free_count(void)
{
    size_t free_count = 0;

    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++)
    {
        if (!used[i])
        {
            free_count++;
        }
    }

    return free_count;
}
