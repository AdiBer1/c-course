#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "course_test.h"
#include "pool.h"

void setUp(void)
{
    pool_init();
}

void tearDown(void) {}

static void test_all_blocks_free_after_init(void)
{
    TEST_ASSERT_EQUAL_size_t(POOL_BLOCK_COUNT, pool_free_count());
}

static void test_alloc_returns_a_block(void)
{
    void *p = pool_alloc();
    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_EQUAL_size_t(POOL_BLOCK_COUNT - 1, pool_free_count());
}

static void test_blocks_are_aligned(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        void *p = pool_alloc();
        TEST_ASSERT_NOT_NULL(p);
        TEST_ASSERT_EQUAL_UINT(0, (uintptr_t)p % _Alignof(max_align_t));
    }
}

static void test_blocks_are_distinct_and_do_not_overlap(void)
{
    uint8_t *blocks[POOL_BLOCK_COUNT];
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        blocks[i] = pool_alloc();
        TEST_ASSERT_NOT_NULL(blocks[i]);
        memset(blocks[i], (int)i, POOL_BLOCK_SIZE);
    }
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        for (size_t b = 0; b < POOL_BLOCK_SIZE; b++) {
            TEST_ASSERT_EQUAL_UINT8((uint8_t)i, blocks[i][b]);
        }
    }
}

static void test_alloc_returns_null_when_exhausted(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        TEST_ASSERT_NOT_NULL(pool_alloc());
    }
    TEST_ASSERT_NULL(pool_alloc());
    TEST_ASSERT_NULL(pool_alloc());
    TEST_ASSERT_EQUAL_size_t(0, pool_free_count());
}

static void test_freed_block_can_be_allocated_again(void)
{
    void *blocks[POOL_BLOCK_COUNT];
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        blocks[i] = pool_alloc();
    }
    TEST_ASSERT_TRUE(pool_free(blocks[3]));
    TEST_ASSERT_EQUAL_size_t(1, pool_free_count());
    TEST_ASSERT_EQUAL_PTR(blocks[3], pool_alloc());
    TEST_ASSERT_NULL(pool_alloc());
}

static void test_free_and_alloc_many_times(void)
{
    for (int round = 0; round < 1000; round++) {
        void *a = pool_alloc();
        void *b = pool_alloc();
        TEST_ASSERT_NOT_NULL(a);
        TEST_ASSERT_NOT_NULL(b);
        TEST_ASSERT_TRUE(pool_free(a));
        TEST_ASSERT_TRUE(pool_free(b));
    }
    TEST_ASSERT_EQUAL_size_t(POOL_BLOCK_COUNT, pool_free_count());
}

static void test_free_null_returns_false(void)
{
    TEST_ASSERT_FALSE(pool_free(NULL));
    TEST_ASSERT_EQUAL_size_t(POOL_BLOCK_COUNT, pool_free_count());
}

static void test_double_free_is_rejected(void)
{
    void *p = pool_alloc();
    TEST_ASSERT_TRUE(pool_free(p));
    TEST_ASSERT_FALSE(pool_free(p));
    TEST_ASSERT_EQUAL_size_t(POOL_BLOCK_COUNT, pool_free_count());
}

static void test_free_of_never_allocated_block_is_rejected(void)
{
    uint8_t *p = pool_alloc();
    uint8_t *q = pool_alloc();
    TEST_ASSERT_TRUE(pool_free(q));
    /* q is free again; p is still in use. */
    TEST_ASSERT_FALSE(pool_free(q));
    TEST_ASSERT_TRUE(pool_free(p));
}

static void test_free_of_foreign_pointer_is_rejected(void)
{
    static uint8_t not_from_pool[POOL_BLOCK_SIZE];
    int local = 0;
    (void)pool_alloc();
    TEST_ASSERT_FALSE(pool_free(not_from_pool));
    TEST_ASSERT_FALSE(pool_free(&local));
    TEST_ASSERT_EQUAL_size_t(POOL_BLOCK_COUNT - 1, pool_free_count());
}

static void test_free_of_pointer_into_middle_of_block_is_rejected(void)
{
    uint8_t *p = pool_alloc();
    TEST_ASSERT_FALSE(pool_free(p + 1));
    TEST_ASSERT_FALSE(pool_free(p + POOL_BLOCK_SIZE / 2));
    TEST_ASSERT_TRUE(pool_free(p));
}

static void test_init_resets_the_pool(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        (void)pool_alloc();
    }
    pool_init();
    TEST_ASSERT_EQUAL_size_t(POOL_BLOCK_COUNT, pool_free_count());
    TEST_ASSERT_NOT_NULL(pool_alloc());
}

int main(int argc, char **argv)
{
    course_parse_args(argc, argv);
    UNITY_BEGIN();
    RUN_TEST(test_all_blocks_free_after_init);
    RUN_TEST(test_alloc_returns_a_block);
    RUN_TEST(test_blocks_are_aligned);
    RUN_TEST(test_blocks_are_distinct_and_do_not_overlap);
    RUN_TEST(test_alloc_returns_null_when_exhausted);
    RUN_TEST(test_freed_block_can_be_allocated_again);
    RUN_TEST(test_free_and_alloc_many_times);
    RUN_TEST(test_free_null_returns_false);
    RUN_TEST(test_double_free_is_rejected);
    RUN_TEST(test_free_of_never_allocated_block_is_rejected);
    RUN_TEST(test_free_of_foreign_pointer_is_rejected);
    RUN_TEST(test_free_of_pointer_into_middle_of_block_is_rejected);
    RUN_TEST(test_init_resets_the_pool);
    return UNITY_END();
}
