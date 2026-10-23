#include <stdint.h>

#include "course_test.h"
#include "sensor_log.h"

void setUp(void)
{
    log_init();
}

void tearDown(void) {}

static void fill(int16_t first, size_t how_many)
{
    for (size_t i = 0; i < how_many; i++) {
        log_add((int16_t)(first + (int16_t)i));
    }
}

/* ---------------------------------------------------------------- add / get */

static void test_new_log_is_empty(void)
{
    TEST_ASSERT_EQUAL_size_t(0, log_count());
}

static void test_add_and_get_in_order(void)
{
    TEST_ASSERT_TRUE(log_add(10));
    TEST_ASSERT_TRUE(log_add(-20));
    TEST_ASSERT_TRUE(log_add(30));
    TEST_ASSERT_EQUAL_size_t(3, log_count());
    TEST_ASSERT_EQUAL_INT16(10, log_get(0));
    TEST_ASSERT_EQUAL_INT16(-20, log_get(1));
    TEST_ASSERT_EQUAL_INT16(30, log_get(2));
}

static void test_add_fails_when_full(void)
{
    for (int i = 0; i < LOG_CAPACITY; i++) {
        TEST_ASSERT_TRUE(log_add((int16_t)i));
    }
    TEST_ASSERT_FALSE(log_add(99));
    TEST_ASSERT_EQUAL_size_t(LOG_CAPACITY, log_count());
}

static void test_full_log_keeps_its_values(void)
{
    fill(100, LOG_CAPACITY);
    log_add(-1);
    TEST_ASSERT_EQUAL_INT16(100, log_get(0));
    TEST_ASSERT_EQUAL_INT16(100 + LOG_CAPACITY - 1, log_get(LOG_CAPACITY - 1));
}

static void test_get_past_the_end_returns_zero(void)
{
    fill(7, LOG_CAPACITY);
    log_init();
    fill(1, 3);
    TEST_ASSERT_EQUAL_INT16(0, log_get(3));
    TEST_ASSERT_EQUAL_INT16(0, log_get(1000));
}

static void test_get_past_the_end_of_full_log_returns_zero(void)
{
    fill(1, LOG_CAPACITY);
    TEST_ASSERT_EQUAL_INT16(0, log_get(LOG_CAPACITY));
}

/* ---------------------------------------------------------------- average */

static void test_average_of_small_values(void)
{
    log_add(1);
    log_add(2);
    log_add(4);
    TEST_ASSERT_EQUAL_INT16(2, log_average());
}

static void test_average_rounds_toward_zero_for_negatives(void)
{
    log_add(-1);
    log_add(-2);
    log_add(-4);
    TEST_ASSERT_EQUAL_INT16(-2, log_average());
}

static void test_average_of_large_values(void)
{
    log_add(30000);
    log_add(30000);
    log_add(30000);
    TEST_ASSERT_EQUAL_INT16(30000, log_average());
    log_init();
    log_add(-32000);
    log_add(-32000);
    TEST_ASSERT_EQUAL_INT16(-32000, log_average());
}

static void test_average_of_empty_log_is_zero(void)
{
    TEST_ASSERT_EQUAL_INT16(0, log_average());
}

/* ---------------------------------------------------------------- copy_last */

static void test_copy_last_gives_newest_values_oldest_first(void)
{
    int16_t dst[8] = {0};
    fill(1, 5);
    TEST_ASSERT_EQUAL_size_t(3, log_copy_last(dst, 8, 3));
    TEST_ASSERT_EQUAL_INT16(3, dst[0]);
    TEST_ASSERT_EQUAL_INT16(4, dst[1]);
    TEST_ASSERT_EQUAL_INT16(5, dst[2]);
}

static void test_copy_last_is_limited_by_count(void)
{
    int16_t dst[8] = {0};
    fill(1, 2);
    TEST_ASSERT_EQUAL_size_t(2, log_copy_last(dst, 8, 6));
    TEST_ASSERT_EQUAL_INT16(1, dst[0]);
    TEST_ASSERT_EQUAL_INT16(2, dst[1]);
}

static void test_copy_last_never_writes_past_dst(void)
{
    int16_t dst[2] = {0};
    fill(1, 5);
    TEST_ASSERT_EQUAL_size_t(2, log_copy_last(dst, 2, 5));
    TEST_ASSERT_EQUAL_INT16(4, dst[0]);
    TEST_ASSERT_EQUAL_INT16(5, dst[1]);
}

static void test_copy_last_of_zero_values(void)
{
    int16_t dst[1] = {123};
    fill(1, 5);
    TEST_ASSERT_EQUAL_size_t(0, log_copy_last(dst, 1, 0));
    TEST_ASSERT_EQUAL_INT16(123, dst[0]);
}

/* ---------------------------------------------------------------- label */

static void test_label_text(void)
{
    TEST_ASSERT_EQUAL_STRING("sensor-log/8", log_label());
}

static void test_label_stays_valid_after_other_calls(void)
{
    const char *first = log_label();
    fill(1, 4);
    (void)log_average();
    TEST_ASSERT_EQUAL_PTR(first, log_label());
    TEST_ASSERT_EQUAL_STRING("sensor-log/8", first);
}

int main(int argc, char **argv)
{
    course_parse_args(argc, argv);
    UNITY_BEGIN();
    RUN_TEST(test_new_log_is_empty);
    RUN_TEST(test_add_and_get_in_order);
    RUN_TEST(test_add_fails_when_full);
    RUN_TEST(test_full_log_keeps_its_values);
    RUN_TEST(test_get_past_the_end_returns_zero);
    RUN_TEST(test_get_past_the_end_of_full_log_returns_zero);
    RUN_TEST(test_average_of_small_values);
    RUN_TEST(test_average_rounds_toward_zero_for_negatives);
    RUN_TEST(test_average_of_large_values);
    RUN_TEST(test_average_of_empty_log_is_zero);
    RUN_TEST(test_copy_last_gives_newest_values_oldest_first);
    RUN_TEST(test_copy_last_is_limited_by_count);
    RUN_TEST(test_copy_last_never_writes_past_dst);
    RUN_TEST(test_copy_last_of_zero_values);
    RUN_TEST(test_label_text);
    RUN_TEST(test_label_stays_valid_after_other_calls);
    return UNITY_END();
}
