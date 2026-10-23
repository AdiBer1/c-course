#include <stdint.h>

#include "course_test.h"
#include "fixed.h"

void setUp(void) {}
void tearDown(void) {}

#define Q(int_part) ((q16_t)((int_part) * 65536))
#define ONE_AND_HALF ((q16_t)0x00018000)
#define HALF ((q16_t)0x00008000)

/* ---------------------------------------------------------------- conversions */

static void test_from_int(void)
{
    TEST_ASSERT_EQUAL_INT32(0x00010000, q16_from_int(1));
    TEST_ASSERT_EQUAL_INT32(-65536, q16_from_int(-1));
    TEST_ASSERT_EQUAL_INT32(0, q16_from_int(0));
    TEST_ASSERT_EQUAL_INT32(0x7FFF0000, q16_from_int(32767));
    TEST_ASSERT_EQUAL_INT32(Q16_MIN, q16_from_int(-32768));
}

static void test_from_int_saturates(void)
{
    TEST_ASSERT_EQUAL_INT32(Q16_MAX, q16_from_int(32768));
    TEST_ASSERT_EQUAL_INT32(Q16_MAX, q16_from_int(INT32_MAX));
    TEST_ASSERT_EQUAL_INT32(Q16_MIN, q16_from_int(-32769));
    TEST_ASSERT_EQUAL_INT32(Q16_MIN, q16_from_int(INT32_MIN));
}

static void test_to_int_rounds_to_nearest(void)
{
    TEST_ASSERT_EQUAL_INT32(3, q16_to_int(Q(3)));
    TEST_ASSERT_EQUAL_INT32(2, q16_to_int(ONE_AND_HALF));
    TEST_ASSERT_EQUAL_INT32(1, q16_to_int(ONE_AND_HALF - 1));
    TEST_ASSERT_EQUAL_INT32(-2, q16_to_int(-ONE_AND_HALF));
    TEST_ASSERT_EQUAL_INT32(-1, q16_to_int(-ONE_AND_HALF + 1));
    TEST_ASSERT_EQUAL_INT32(0, q16_to_int(HALF - 1));
}

static void test_to_int_at_the_limits(void)
{
    TEST_ASSERT_EQUAL_INT32(32768, q16_to_int(Q16_MAX));
    TEST_ASSERT_EQUAL_INT32(-32768, q16_to_int(Q16_MIN));
}

static void test_from_milli(void)
{
    TEST_ASSERT_EQUAL_INT32(98304, q16_from_milli(1500));
    TEST_ASSERT_EQUAL_INT32(-16384, q16_from_milli(-250));
    TEST_ASSERT_EQUAL_INT32(66, q16_from_milli(1));   /* 65.536 -> 66 */
    TEST_ASSERT_EQUAL_INT32(-66, q16_from_milli(-1));
    TEST_ASSERT_EQUAL_INT32(2147483582, q16_from_milli(32767999));
}

static void test_from_milli_saturates(void)
{
    TEST_ASSERT_EQUAL_INT32(Q16_MAX, q16_from_milli(32768000));
    TEST_ASSERT_EQUAL_INT32(Q16_MAX, q16_from_milli(40000000));
    TEST_ASSERT_EQUAL_INT32(Q16_MIN, q16_from_milli(-40000000));
    TEST_ASSERT_EQUAL_INT32(Q16_MIN, q16_from_milli(INT32_MIN));
}

static void test_to_milli(void)
{
    TEST_ASSERT_EQUAL_INT32(1500, q16_to_milli(98304));
    TEST_ASSERT_EQUAL_INT32(0, q16_to_milli(1));    /* 0.015 -> 0 */
    TEST_ASSERT_EQUAL_INT32(1, q16_to_milli(33));   /* 0.5035 -> 1 */
    TEST_ASSERT_EQUAL_INT32(-1, q16_to_milli(-33));
    TEST_ASSERT_EQUAL_INT32(333, q16_to_milli(21845));
    TEST_ASSERT_EQUAL_INT32(32768000, q16_to_milli(Q16_MAX));
    TEST_ASSERT_EQUAL_INT32(-32768000, q16_to_milli(Q16_MIN));
}

/* ---------------------------------------------------------------- add / sub */

static void test_add_and_sub(void)
{
    TEST_ASSERT_EQUAL_INT32(q16_from_milli(3750), q16_add(ONE_AND_HALF, q16_from_milli(2250)));
    TEST_ASSERT_EQUAL_INT32(q16_from_milli(-750), q16_sub(ONE_AND_HALF, q16_from_milli(2250)));
}

static void test_add_saturates(void)
{
    TEST_ASSERT_EQUAL_INT32(Q16_MAX, q16_add(Q16_MAX, 1));
    TEST_ASSERT_EQUAL_INT32(Q16_MAX, q16_add(Q(20000), Q(20000)));
    TEST_ASSERT_EQUAL_INT32(Q16_MIN, q16_add(Q16_MIN, -1));
    TEST_ASSERT_EQUAL_INT32(-1, q16_add(Q16_MAX, Q16_MIN));
}

static void test_sub_saturates(void)
{
    TEST_ASSERT_EQUAL_INT32(Q16_MIN, q16_sub(Q16_MIN, 1));
    TEST_ASSERT_EQUAL_INT32(Q16_MAX, q16_sub(0, Q16_MIN));
    TEST_ASSERT_EQUAL_INT32(Q16_MAX, q16_sub(Q(20000), Q(-20000)));
}

/* ---------------------------------------------------------------- mul */

static void test_mul_simple_values(void)
{
    TEST_ASSERT_EQUAL_INT32(Q(3), q16_mul(ONE_AND_HALF, Q(2)));
    TEST_ASSERT_EQUAL_INT32(Q(-3), q16_mul(-ONE_AND_HALF, Q(2)));
    TEST_ASSERT_EQUAL_INT32(0x4000, q16_mul(HALF, HALF)); /* 0.25 */
    TEST_ASSERT_EQUAL_INT32(Q(6), q16_mul(Q(-2), Q(-3)));
}

static void test_mul_rounds_to_nearest(void)
{
    TEST_ASSERT_EQUAL_INT32(1, q16_mul(1, HALF));       /* 0.5 LSB -> 1 */
    TEST_ASSERT_EQUAL_INT32(-1, q16_mul(-1, HALF));     /* -0.5 LSB -> -1 */
    TEST_ASSERT_EQUAL_INT32(0, q16_mul(1, HALF - 1));   /* 0.49999 LSB -> 0 */
}

static void test_mul_needs_64_bit_intermediate(void)
{
    /* 100.0 * 100.0 = 10000.0: the raw product is about 4.3e13 */
    TEST_ASSERT_EQUAL_INT32(Q(10000), q16_mul(Q(100), Q(100)));
}

static void test_mul_saturates(void)
{
    TEST_ASSERT_EQUAL_INT32(Q16_MAX, q16_mul(Q(200), Q(200)));
    TEST_ASSERT_EQUAL_INT32(Q16_MIN, q16_mul(Q(-200), Q(200)));
    TEST_ASSERT_EQUAL_INT32(Q16_MAX, q16_mul(Q16_MIN, Q16_MIN));
}

/* ---------------------------------------------------------------- div */

static void test_div_simple_values(void)
{
    TEST_ASSERT_EQUAL_INT32(ONE_AND_HALF, q16_div(Q(3), Q(2)));
    TEST_ASSERT_EQUAL_INT32(Q(-4), q16_div(Q(8), Q(-2)));
    TEST_ASSERT_EQUAL_INT32(-229376, q16_div(7, -2)); /* -3.5 */
}

static void test_div_rounds_to_nearest(void)
{
    TEST_ASSERT_EQUAL_INT32(21845, q16_div(Q(1), Q(3)));   /* 21845.33 */
    TEST_ASSERT_EQUAL_INT32(43691, q16_div(Q(2), Q(3)));   /* 43690.67 */
    TEST_ASSERT_EQUAL_INT32(-21845, q16_div(Q(-1), Q(3)));
    TEST_ASSERT_EQUAL_INT32(-43691, q16_div(Q(2), Q(-3)));
}

static void test_div_by_zero(void)
{
    TEST_ASSERT_EQUAL_INT32(Q16_MAX, q16_div(Q(5), 0));
    TEST_ASSERT_EQUAL_INT32(Q16_MIN, q16_div(Q(-5), 0));
    TEST_ASSERT_EQUAL_INT32(0, q16_div(0, 0));
}

static void test_div_saturates(void)
{
    TEST_ASSERT_EQUAL_INT32(Q16_MAX, q16_div(Q(30000), 65)); /* 30000 / 0.001 */
    TEST_ASSERT_EQUAL_INT32(Q16_MAX, q16_div(Q16_MIN, Q(-1))); /* 32768.0 does not fit */
    TEST_ASSERT_EQUAL_INT32(Q16_MIN, q16_div(Q(-30000), 65));
}

/* ---------------------------------------------------------------- a real use */

static void test_low_pass_filter_converges(void)
{
    /* y += alpha * (x - y), alpha = 0.125, x = 100.0: y approaches 100 */
    const q16_t alpha = q16_from_milli(125);
    const q16_t x = Q(100);
    q16_t y = 0;
    for (int i = 0; i < 200; i++) {
        y = q16_add(y, q16_mul(alpha, q16_sub(x, y)));
    }
    TEST_ASSERT_INT32_WITHIN(8, x, y);
    TEST_ASSERT_EQUAL_INT32(100, q16_to_int(y));
}

int main(int argc, char **argv)
{
    course_parse_args(argc, argv);
    UNITY_BEGIN();
    RUN_TEST(test_from_int);
    RUN_TEST(test_from_int_saturates);
    RUN_TEST(test_to_int_rounds_to_nearest);
    RUN_TEST(test_to_int_at_the_limits);
    RUN_TEST(test_from_milli);
    RUN_TEST(test_from_milli_saturates);
    RUN_TEST(test_to_milli);
    RUN_TEST(test_add_and_sub);
    RUN_TEST(test_add_saturates);
    RUN_TEST(test_sub_saturates);
    RUN_TEST(test_mul_simple_values);
    RUN_TEST(test_mul_rounds_to_nearest);
    RUN_TEST(test_mul_needs_64_bit_intermediate);
    RUN_TEST(test_mul_saturates);
    RUN_TEST(test_div_simple_values);
    RUN_TEST(test_div_rounds_to_nearest);
    RUN_TEST(test_div_by_zero);
    RUN_TEST(test_div_saturates);
    RUN_TEST(test_low_pass_filter_converges);
    return UNITY_END();
}
