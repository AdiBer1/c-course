#include <stdint.h>

#include "course_test.h"
#include "saturate.h"

void setUp(void) {}
void tearDown(void) {}

/* ---------------------------------------------------------------- uint8_t */

static void test_add_u8_small_values(void)
{
    TEST_ASSERT_EQUAL_UINT8(30, sat_add_u8(10, 20));
}

static void test_add_u8_exactly_max(void)
{
    TEST_ASSERT_EQUAL_UINT8(255, sat_add_u8(200, 55));
}

static void test_add_u8_overflow_saturates_at_255(void)
{
    TEST_ASSERT_EQUAL_UINT8(255, sat_add_u8(200, 100));
    TEST_ASSERT_EQUAL_UINT8(255, sat_add_u8(255, 255));
    TEST_ASSERT_EQUAL_UINT8(255, sat_add_u8(1, 255));
}

static void test_add_u8_with_zero(void)
{
    TEST_ASSERT_EQUAL_UINT8(77, sat_add_u8(77, 0));
    TEST_ASSERT_EQUAL_UINT8(255, sat_add_u8(0, 255));
}

static void test_sub_u8_normal(void)
{
    TEST_ASSERT_EQUAL_UINT8(90, sat_sub_u8(100, 10));
}

static void test_sub_u8_to_exactly_zero(void)
{
    TEST_ASSERT_EQUAL_UINT8(0, sat_sub_u8(42, 42));
}

static void test_sub_u8_underflow_saturates_at_0(void)
{
    TEST_ASSERT_EQUAL_UINT8(0, sat_sub_u8(10, 20));
    TEST_ASSERT_EQUAL_UINT8(0, sat_sub_u8(0, 255));
}

static void test_sub_u8_from_max(void)
{
    TEST_ASSERT_EQUAL_UINT8(254, sat_sub_u8(255, 1));
}

/* ---------------------------------------------------------------- int16_t */

static void test_add_i16_small_values(void)
{
    TEST_ASSERT_EQUAL_INT16(-5, sat_add_i16(10, -15));
}

static void test_add_i16_positive_overflow_saturates_at_max(void)
{
    TEST_ASSERT_EQUAL_INT16(INT16_MAX, sat_add_i16(30000, 10000));
    TEST_ASSERT_EQUAL_INT16(INT16_MAX, sat_add_i16(INT16_MAX, 1));
    TEST_ASSERT_EQUAL_INT16(INT16_MAX, sat_add_i16(INT16_MAX, INT16_MAX));
}

static void test_add_i16_negative_overflow_saturates_at_min(void)
{
    TEST_ASSERT_EQUAL_INT16(INT16_MIN, sat_add_i16(-30000, -10000));
    TEST_ASSERT_EQUAL_INT16(INT16_MIN, sat_add_i16(INT16_MIN, -1));
    TEST_ASSERT_EQUAL_INT16(INT16_MIN, sat_add_i16(INT16_MIN, INT16_MIN));
}

static void test_add_i16_exact_limits_are_not_clamped(void)
{
    TEST_ASSERT_EQUAL_INT16(INT16_MAX, sat_add_i16(32000, 767));
    TEST_ASSERT_EQUAL_INT16(INT16_MIN, sat_add_i16(-32000, -768));
}

static void test_add_i16_opposite_signs_never_saturate(void)
{
    TEST_ASSERT_EQUAL_INT16(-1, sat_add_i16(INT16_MAX, INT16_MIN));
}

static void test_sub_i16_normal(void)
{
    TEST_ASSERT_EQUAL_INT16(-300, sat_sub_i16(-100, 200));
}

static void test_sub_i16_overflow_saturates(void)
{
    TEST_ASSERT_EQUAL_INT16(INT16_MAX, sat_sub_i16(30000, -10000));
    TEST_ASSERT_EQUAL_INT16(INT16_MIN, sat_sub_i16(-30000, 10000));
}

static void test_sub_i16_of_min_value(void)
{
    /* -INT16_MIN does not fit in int16_t: 0 - (-32768) must saturate. */
    TEST_ASSERT_EQUAL_INT16(INT16_MAX, sat_sub_i16(0, INT16_MIN));
    TEST_ASSERT_EQUAL_INT16(0, sat_sub_i16(INT16_MIN, INT16_MIN));
}

int main(int argc, char **argv)
{
    course_parse_args(argc, argv);
    UNITY_BEGIN();
    RUN_TEST(test_add_u8_small_values);
    RUN_TEST(test_add_u8_exactly_max);
    RUN_TEST(test_add_u8_overflow_saturates_at_255);
    RUN_TEST(test_add_u8_with_zero);
    RUN_TEST(test_sub_u8_normal);
    RUN_TEST(test_sub_u8_to_exactly_zero);
    RUN_TEST(test_sub_u8_underflow_saturates_at_0);
    RUN_TEST(test_sub_u8_from_max);
    RUN_TEST(test_add_i16_small_values);
    RUN_TEST(test_add_i16_positive_overflow_saturates_at_max);
    RUN_TEST(test_add_i16_negative_overflow_saturates_at_min);
    RUN_TEST(test_add_i16_exact_limits_are_not_clamped);
    RUN_TEST(test_add_i16_opposite_signs_never_saturate);
    RUN_TEST(test_sub_i16_normal);
    RUN_TEST(test_sub_i16_overflow_saturates);
    RUN_TEST(test_sub_i16_of_min_value);
    return UNITY_END();
}
