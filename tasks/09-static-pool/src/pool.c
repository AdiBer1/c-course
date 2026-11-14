
#include "pool.h"

#include <stdint.h>

static union
{
    max_align_t align;
    uint8_t data[POOL_BLOCK_COUNT * POOL_BLOCK_SIZE];
} pool;

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
            return &pool.data[i * POOL_BLOCK_SIZE];
        }
    }

    return NULL;
}

bool pool_free(void *block)
{
    if (block == NULL)
    {
        return false;
    }

    uintptr_t start = (uintptr_t)&pool.data[0];
    uintptr_t end = start + sizeof(pool.data);
    uintptr_t address = (uintptr_t)block;

    if (address < start || address >= end)
    {
        return false;
    }

    uintptr_t offset = address - start;

    if (offset % POOL_BLOCK_SIZE != 0)
    {
        return false;
    }

    size_t index = (size_t)(offset / POOL_BLOCK_SIZE);

    if (!used[index])
    {
        return false;
    }

    used[index] = false;
    return true;
}

size_t pool_free_count(void)
{
    size_t count = 0;

    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++)
    {
        if (!used[i])
        {
            count++;
        }
    }

    return count;
}
