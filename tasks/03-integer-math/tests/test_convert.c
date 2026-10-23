#include <stdint.h>

#include "convert.h"
#include "course_test.h"

void setUp(void) {}
void tearDown(void) {}

/* ---------------------------------------------------------------- C -> F */

static void test_c_to_f_freezing_point(void)
{
    TEST_ASSERT_EQUAL_INT32(320, c10_to_f10(0));
}

static void test_c_to_f_boiling_point(void)
{
    TEST_ASSERT_EQUAL_INT32(2120, c10_to_f10(1000));
}

static void test_c_to_f_minus_forty_is_the_same(void)
{
    TEST_ASSERT_EQUAL_INT32(-400, c10_to_f10(-400));
}

static void test_c_to_f_rounds_to_nearest_not_down(void)
{
    /* 0.1 C = 32.18 F -> 32.2 (truncation would give 32.1) */
    TEST_ASSERT_EQUAL_INT32(322, c10_to_f10(1));
    /* 0.3 C = 32.54 F -> 32.5 */
    TEST_ASSERT_EQUAL_INT32(325, c10_to_f10(3));
}

static void test_c_to_f_rounds_negative_values_correctly(void)
{
    /* -0.1 C = 31.82 F -> 31.8 */
    TEST_ASSERT_EQUAL_INT32(318, c10_to_f10(-1));
    /* -20.1 C = -4.18 F -> -4.2 */
    TEST_ASSERT_EQUAL_INT32(-42, c10_to_f10(-201));
    /* -30.3 C = -22.54 F -> -22.5 */
    TEST_ASSERT_EQUAL_INT32(-225, c10_to_f10(-303));
}

static void test_c_to_f_range_limits(void)
{
    TEST_ASSERT_EQUAL_INT32(180320, c10_to_f10(100000));
    TEST_ASSERT_EQUAL_INT32(-179680, c10_to_f10(-100000));
}

/* ---------------------------------------------------------------- F -> C */

static void test_f_to_c_freezing_and_boiling(void)
{
    TEST_ASSERT_EQUAL_INT32(0, f10_to_c10(320));
    TEST_ASSERT_EQUAL_INT32(1000, f10_to_c10(2120));
}

static void test_f_to_c_zero_fahrenheit(void)
{
    /* 0.0 F = -17.777 C -> -17.8 */
    TEST_ASSERT_EQUAL_INT32(-178, f10_to_c10(0));
}

static void test_f_to_c_rounds_to_nearest(void)
{
    /* 98.6 F = 37.0 C exactly */
    TEST_ASSERT_EQUAL_INT32(370, f10_to_c10(986));
    /* 50.5 F = 10.277 C -> 10.3 */
    TEST_ASSERT_EQUAL_INT32(103, f10_to_c10(505));
    /* 32.1 F = 0.0555 C -> 0.1 */
    TEST_ASSERT_EQUAL_INT32(1, f10_to_c10(321));
}

static void test_f_to_c_rounds_negative_values_correctly(void)
{
    /* 31.9 F = -0.0555 C -> -0.1 */
    TEST_ASSERT_EQUAL_INT32(-1, f10_to_c10(319));
    /* -40.0 F = -40.0 C */
    TEST_ASSERT_EQUAL_INT32(-400, f10_to_c10(-400));
    /* -10.0 F = -23.33 C -> -23.3 */
    TEST_ASSERT_EQUAL_INT32(-233, f10_to_c10(-100));
}

static void test_round_trip_is_stable(void)
{
    for (int32_t c10 = -500; c10 <= 500; c10 += 7) {
        TEST_ASSERT_EQUAL_INT32(c10, f10_to_c10(c10_to_f10(c10)));
    }
}

/* ---------------------------------------------------------------- ADC */

static void test_adc_zero_and_full_scale(void)
{
    TEST_ASSERT_EQUAL_UINT16(0, adc_to_mv(0));
    TEST_ASSERT_EQUAL_UINT16(3300, adc_to_mv(4095));
}

static void test_adc_midpoint_rounds_to_nearest(void)
{
    /* 2048 * 3300 / 4095 = 1650.40 -> 1650 */
    TEST_ASSERT_EQUAL_UINT16(1650, adc_to_mv(2048));
    /* 1 * 3300 / 4095 = 0.806 -> 1 (truncation would give 0) */
    TEST_ASSERT_EQUAL_UINT16(1, adc_to_mv(1));
    /* 4094 * 3300 / 4095 = 3299.19 -> 3299 */
    TEST_ASSERT_EQUAL_UINT16(3299, adc_to_mv(4094));
    /* 1000 * 3300 / 4095 = 805.86 -> 806 */
    TEST_ASSERT_EQUAL_UINT16(806, adc_to_mv(1000));
}

static void test_adc_values_above_12_bits_are_clamped(void)
{
    TEST_ASSERT_EQUAL_UINT16(3300, adc_to_mv(4096));
    TEST_ASSERT_EQUAL_UINT16(3300, adc_to_mv(UINT16_MAX));
}

static void test_adc_is_monotonic(void)
{
    uint16_t previous = adc_to_mv(0);
    for (uint16_t raw = 1; raw <= 4095; raw++) {
        uint16_t mv = adc_to_mv(raw);
        TEST_ASSERT_TRUE(mv >= previous);
        previous = mv;
    }
}

int main(int argc, char **argv)
{
    course_parse_args(argc, argv);
    UNITY_BEGIN();
    RUN_TEST(test_c_to_f_freezing_point);
    RUN_TEST(test_c_to_f_boiling_point);
    RUN_TEST(test_c_to_f_minus_forty_is_the_same);
    RUN_TEST(test_c_to_f_rounds_to_nearest_not_down);
    RUN_TEST(test_c_to_f_rounds_negative_values_correctly);
    RUN_TEST(test_c_to_f_range_limits);
    RUN_TEST(test_f_to_c_freezing_and_boiling);
    RUN_TEST(test_f_to_c_zero_fahrenheit);
    RUN_TEST(test_f_to_c_rounds_to_nearest);
    RUN_TEST(test_f_to_c_rounds_negative_values_correctly);
    RUN_TEST(test_round_trip_is_stable);
    RUN_TEST(test_adc_zero_and_full_scale);
    RUN_TEST(test_adc_midpoint_rounds_to_nearest);
    RUN_TEST(test_adc_values_above_12_bits_are_clamped);
    RUN_TEST(test_adc_is_monotonic);
    return UNITY_END();
}
