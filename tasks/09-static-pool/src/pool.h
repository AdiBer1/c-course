/*
 * pool.h - a fixed-size block allocator over a static array.
 *
 * Firmware that needs "dynamic" objects (message buffers, timers, network
 * packets) but must not use malloc often uses a POOL: a static array cut
 * into equal blocks, plus bookkeeping of which blocks are in use.
 *
 *   - allocation and free are fast and take a bounded time
 *   - no fragmentation: any free block fits any request
 *   - the worst case is known at compile time: at most POOL_BLOCK_COUNT
 *
 * Constraints: no malloc/calloc/realloc/free (checked). All storage is static.
 *
 * Do not change this file.
 */
#ifndef POOL_H
#define POOL_H

#include <stdbool.h>
#include <stddef.h>

#define POOL_BLOCK_SIZE  32u /* bytes per block */
#define POOL_BLOCK_COUNT 8u  /* number of blocks */

/* Mark every block free. Must be called before first use; may be called again to reset. */
void pool_init(void);

/*
 * Get a free block of POOL_BLOCK_SIZE bytes, or NULL if none is left.
 * Every block address is aligned for any type (a multiple of _Alignof(max_align_t)).
 */
void *pool_alloc(void);

/*
 * Give a block back. Returns true on success. Returns false, and changes
 * nothing, if `block` is:
 *   - NULL
 *   - not the start of one of the pool's blocks (outside the pool, or
 *     pointing into the middle of a block)
 *   - already free (a "double free")
 */
bool pool_free(void *block);

/* Number of blocks currently free. */
size_t pool_free_count(void);

#endif /* POOL_H */
