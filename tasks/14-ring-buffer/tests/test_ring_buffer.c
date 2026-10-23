#include <stdint.h>
#include <string.h>

#include "course_test.h"
#include "ring_buffer.h"

#define SIZE 4u

static uint8_t storage[SIZE];
static ring_buffer_t rb;

void setUp(void)
{
    memset(storage, 0xEE, sizeof storage);
    TEST_ASSERT_EQUAL_INT(RB_OK, rb_init(&rb, storage, SIZE));
}

void tearDown(void) {}

/* ---------------------------------------------------------------- init */

static void test_init_gives_an_empty_buffer(void)
{
    TEST_ASSERT_TRUE(rb_is_empty(&rb));
    TEST_ASSERT_FALSE(rb_is_full(&rb));
    TEST_ASSERT_EQUAL_size_t(0, rb_count(&rb));
    TEST_ASSERT_EQUAL_size_t(SIZE, rb_free(&rb));
}

static void test_init_rejects_bad_arguments(void)
{
    ring_buffer_t other;
    TEST_ASSERT_EQUAL_INT(RB_ERR_NULL, rb_init(NULL, storage, SIZE));
    TEST_ASSERT_EQUAL_INT(RB_ERR_NULL, rb_init(&other, NULL, SIZE));
    TEST_ASSERT_EQUAL_INT(RB_ERR_SIZE, rb_init(&other, storage, 0));
}

/* ---------------------------------------------------------------- push / pop */

static void test_push_then_pop_one_byte(void)
{
    uint8_t out = 0;
    TEST_ASSERT_EQUAL_INT(RB_OK, rb_push(&rb, 0x42));
    TEST_ASSERT_EQUAL_size_t(1, rb_count(&rb));
    TEST_ASSERT_EQUAL_INT(RB_OK, rb_pop(&rb, &out));
    TEST_ASSERT_EQUAL_HEX8(0x42, out);
    TEST_ASSERT_TRUE(rb_is_empty(&rb));
}

static void test_bytes_come_out_in_fifo_order(void)
{
    uint8_t out = 0;
    rb_push(&rb, 1);
    rb_push(&rb, 2);
    rb_push(&rb, 3);
    rb_pop(&rb, &out);
    TEST_ASSERT_EQUAL_UINT8(1, out);
    rb_pop(&rb, &out);
    TEST_ASSERT_EQUAL_UINT8(2, out);
    rb_pop(&rb, &out);
    TEST_ASSERT_EQUAL_UINT8(3, out);
}

static void test_all_size_bytes_can_be_used(void)
{
    for (uint8_t i = 0; i < SIZE; i++) {
        TEST_ASSERT_EQUAL_INT(RB_OK, rb_push(&rb, i));
    }
    TEST_ASSERT_TRUE(rb_is_full(&rb));
    TEST_ASSERT_EQUAL_size_t(0, rb_free(&rb));
    TEST_ASSERT_EQUAL_size_t(SIZE, rb_count(&rb));
}

static void test_push_when_full_returns_error_and_keeps_data(void)
{
    uint8_t out = 0;
    for (uint8_t i = 0; i < SIZE; i++) {
        rb_push(&rb, (uint8_t)(10 + i));
    }
    TEST_ASSERT_EQUAL_INT(RB_ERR_FULL, rb_push(&rb, 99));
    TEST_ASSERT_EQUAL_size_t(SIZE, rb_count(&rb));
    rb_pop(&rb, &out);
    TEST_ASSERT_EQUAL_UINT8(10, out);
}

static void test_pop_when_empty_returns_error(void)
{
    uint8_t out = 0x77;
    TEST_ASSERT_EQUAL_INT(RB_ERR_EMPTY, rb_pop(&rb, &out));
    TEST_ASSERT_EQUAL_HEX8(0x77, out);
}

static void test_indices_wrap_around(void)
{
    uint8_t out = 0;
    /* Move head and tail around the array many times. */
    for (unsigned round = 0; round < 25; round++) {
        TEST_ASSERT_EQUAL_INT(RB_OK, rb_push(&rb, (uint8_t)round));
        TEST_ASSERT_EQUAL_INT(RB_OK, rb_push(&rb, (uint8_t)(round + 100)));
        TEST_ASSERT_EQUAL_INT(RB_OK, rb_pop(&rb, &out));
        TEST_ASSERT_EQUAL_UINT8((uint8_t)round, out);
        TEST_ASSERT_EQUAL_INT(RB_OK, rb_pop(&rb, &out));
        TEST_ASSERT_EQUAL_UINT8((uint8_t)(round + 100), out);
    }
    TEST_ASSERT_TRUE(rb_is_empty(&rb));
}

static void test_full_after_wrap_around(void)
{
    uint8_t out = 0;
    rb_push(&rb, 1);
    rb_push(&rb, 2);
    rb_pop(&rb, &out);
    rb_pop(&rb, &out);
    for (uint8_t i = 0; i < SIZE; i++) {
        TEST_ASSERT_EQUAL_INT(RB_OK, rb_push(&rb, (uint8_t)(i + 50)));
    }
    TEST_ASSERT_EQUAL_INT(RB_ERR_FULL, rb_push(&rb, 0));
    for (uint8_t i = 0; i < SIZE; i++) {
        TEST_ASSERT_EQUAL_INT(RB_OK, rb_pop(&rb, &out));
        TEST_ASSERT_EQUAL_UINT8(i + 50, out);
    }
    TEST_ASSERT_EQUAL_INT(RB_ERR_EMPTY, rb_pop(&rb, &out));
}

static void test_never_writes_outside_storage(void)
{
    uint8_t area[SIZE + 2];
    ring_buffer_t small;
    memset(area, 0xAB, sizeof area);
    rb_init(&small, area + 1, SIZE);
    for (unsigned i = 0; i < 3 * SIZE; i++) {
        rb_push(&small, 0x00);
        if (i % 2u == 0u) {
            uint8_t out;
            rb_pop(&small, &out);
        }
    }
    TEST_ASSERT_EQUAL_HEX8(0xAB, area[0]);
    TEST_ASSERT_EQUAL_HEX8(0xAB, area[SIZE + 1]);
}

/* ---------------------------------------------------------------- peek / clear */

static void test_peek_does_not_remove(void)
{
    uint8_t out = 0;
    rb_push(&rb, 7);
    rb_push(&rb, 8);
    TEST_ASSERT_EQUAL_INT(RB_OK, rb_peek(&rb, &out));
    TEST_ASSERT_EQUAL_UINT8(7, out);
    TEST_ASSERT_EQUAL_size_t(2, rb_count(&rb));
    TEST_ASSERT_EQUAL_INT(RB_OK, rb_peek(&rb, &out));
    TEST_ASSERT_EQUAL_UINT8(7, out);
}

static void test_peek_when_empty_returns_error(void)
{
    uint8_t out = 0x77;
    TEST_ASSERT_EQUAL_INT(RB_ERR_EMPTY, rb_peek(&rb, &out));
    TEST_ASSERT_EQUAL_HEX8(0x77, out);
}

static void test_clear_empties_the_buffer(void)
{
    rb_push(&rb, 1);
    rb_push(&rb, 2);
    rb_clear(&rb);
    TEST_ASSERT_TRUE(rb_is_empty(&rb));
    TEST_ASSERT_EQUAL_size_t(SIZE, rb_free(&rb));
    TEST_ASSERT_EQUAL_INT(RB_OK, rb_push(&rb, 3));
}

/* ---------------------------------------------------------------- write / read */

static void test_write_stops_when_full(void)
{
    const uint8_t data[] = {1, 2, 3, 4, 5, 6};
    TEST_ASSERT_EQUAL_size_t(SIZE, rb_write(&rb, data, sizeof data));
    TEST_ASSERT_TRUE(rb_is_full(&rb));
}

static void test_read_stops_when_empty(void)
{
    const uint8_t data[] = {9, 8, 7};
    uint8_t dst[8] = {0};
    rb_write(&rb, data, sizeof data);
    TEST_ASSERT_EQUAL_size_t(3, rb_read(&rb, dst, sizeof dst));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(data, dst, 3);
    TEST_ASSERT_TRUE(rb_is_empty(&rb));
}

static void test_read_partial(void)
{
    const uint8_t data[] = {1, 2, 3};
    uint8_t dst[2] = {0};
    uint8_t out = 0;
    rb_write(&rb, data, sizeof data);
    TEST_ASSERT_EQUAL_size_t(2, rb_read(&rb, dst, 2));
    TEST_ASSERT_EQUAL_UINT8(1, dst[0]);
    TEST_ASSERT_EQUAL_UINT8(2, dst[1]);
    rb_pop(&rb, &out);
    TEST_ASSERT_EQUAL_UINT8(3, out);
}

/* ---------------------------------------------------------------- NULL */

static void test_null_buffer_is_handled_everywhere(void)
{
    uint8_t out = 0;
    uint8_t data[2] = {0};
    TEST_ASSERT_EQUAL_INT(RB_ERR_NULL, rb_push(NULL, 1));
    TEST_ASSERT_EQUAL_INT(RB_ERR_NULL, rb_pop(NULL, &out));
    TEST_ASSERT_EQUAL_INT(RB_ERR_NULL, rb_pop(&rb, NULL));
    TEST_ASSERT_EQUAL_INT(RB_ERR_NULL, rb_peek(NULL, &out));
    TEST_ASSERT_EQUAL_size_t(0, rb_count(NULL));
    TEST_ASSERT_EQUAL_size_t(0, rb_free(NULL));
    TEST_ASSERT_TRUE(rb_is_empty(NULL));
    TEST_ASSERT_FALSE(rb_is_full(NULL));
    rb_clear(NULL);
    TEST_ASSERT_EQUAL_size_t(0, rb_write(NULL, data, 2));
    TEST_ASSERT_EQUAL_size_t(0, rb_write(&rb, NULL, 2));
    TEST_ASSERT_EQUAL_size_t(0, rb_read(NULL, data, 2));
    TEST_ASSERT_EQUAL_size_t(0, rb_read(&rb, NULL, 2));
}

static void test_capacity_of_one(void)
{
    uint8_t one[1];
    ring_buffer_t tiny;
    uint8_t out = 0;
    TEST_ASSERT_EQUAL_INT(RB_OK, rb_init(&tiny, one, 1));
    TEST_ASSERT_EQUAL_INT(RB_OK, rb_push(&tiny, 5));
    TEST_ASSERT_EQUAL_INT(RB_ERR_FULL, rb_push(&tiny, 6));
    TEST_ASSERT_EQUAL_INT(RB_OK, rb_pop(&tiny, &out));
    TEST_ASSERT_EQUAL_UINT8(5, out);
    TEST_ASSERT_EQUAL_INT(RB_OK, rb_push(&tiny, 7));
    TEST_ASSERT_EQUAL_INT(RB_OK, rb_pop(&tiny, &out));
    TEST_ASSERT_EQUAL_UINT8(7, out);
}

int main(int argc, char **argv)
{
    course_parse_args(argc, argv);
    UNITY_BEGIN();
    RUN_TEST(test_init_gives_an_empty_buffer);
    RUN_TEST(test_init_rejects_bad_arguments);
    RUN_TEST(test_push_then_pop_one_byte);
    RUN_TEST(test_bytes_come_out_in_fifo_order);
    RUN_TEST(test_all_size_bytes_can_be_used);
    RUN_TEST(test_push_when_full_returns_error_and_keeps_data);
    RUN_TEST(test_pop_when_empty_returns_error);
    RUN_TEST(test_indices_wrap_around);
    RUN_TEST(test_full_after_wrap_around);
    RUN_TEST(test_never_writes_outside_storage);
    RUN_TEST(test_peek_does_not_remove);
    RUN_TEST(test_peek_when_empty_returns_error);
    RUN_TEST(test_clear_empties_the_buffer);
    RUN_TEST(test_write_stops_when_full);
    RUN_TEST(test_read_stops_when_empty);
    RUN_TEST(test_read_partial);
    RUN_TEST(test_null_buffer_is_handled_everywhere);
    RUN_TEST(test_capacity_of_one);
    return UNITY_END();
}
