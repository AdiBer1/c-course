#include <stddef.h>
#include <stdint.h>

#include "course_test.h"
#include "outparams.h"

void setUp(void) {}
void tearDown(void) {}

/* ---------------------------------------------------------------- swap */

static void test_swap_exchanges_values(void)
{
    int32_t a = 3, b = -8;
    swap_i32(&a, &b);
    TEST_ASSERT_EQUAL_INT32(-8, a);
    TEST_ASSERT_EQUAL_INT32(3, b);
}

static void test_swap_same_variable_keeps_value(void)
{
    int32_t a = 42;
    swap_i32(&a, &a);
    TEST_ASSERT_EQUAL_INT32(42, a);
}

static void test_swap_with_null_does_nothing(void)
{
    int32_t a = 1;
    swap_i32(&a, NULL);
    swap_i32(NULL, &a);
    swap_i32(NULL, NULL);
    TEST_ASSERT_EQUAL_INT32(1, a);
}

/* ---------------------------------------------------------------- min/max */

static void test_min_max_of_mixed_values(void)
{
    const int16_t v[] = {4, -7, 19, 0, 3};
    int16_t lo = 0, hi = 0;
    TEST_ASSERT_EQUAL_INT(OP_OK, min_max_i16(v, 5, &lo, &hi));
    TEST_ASSERT_EQUAL_INT16(-7, lo);
    TEST_ASSERT_EQUAL_INT16(19, hi);
}

static void test_min_max_of_single_value(void)
{
    const int16_t v[] = {-300};
    int16_t lo = 0, hi = 0;
    TEST_ASSERT_EQUAL_INT(OP_OK, min_max_i16(v, 1, &lo, &hi));
    TEST_ASSERT_EQUAL_INT16(-300, lo);
    TEST_ASSERT_EQUAL_INT16(-300, hi);
}

static void test_min_max_at_the_ends_and_extremes(void)
{
    const int16_t v[] = {INT16_MAX, 5, 6, INT16_MIN};
    int16_t lo = 0, hi = 0;
    TEST_ASSERT_EQUAL_INT(OP_OK, min_max_i16(v, 4, &lo, &hi));
    TEST_ASSERT_EQUAL_INT16(INT16_MIN, lo);
    TEST_ASSERT_EQUAL_INT16(INT16_MAX, hi);
}

static void test_min_max_reads_only_len_elements(void)
{
    const int16_t v[] = {10, 20, 30, 999};
    int16_t lo = 0, hi = 0;
    TEST_ASSERT_EQUAL_INT(OP_OK, min_max_i16(v, 3, &lo, &hi));
    TEST_ASSERT_EQUAL_INT16(30, hi);
}

static void test_min_max_empty_returns_error_and_keeps_outputs(void)
{
    const int16_t v[] = {1};
    int16_t lo = 111, hi = 222;
    TEST_ASSERT_EQUAL_INT(OP_ERR_EMPTY, min_max_i16(v, 0, &lo, &hi));
    TEST_ASSERT_EQUAL_INT16(111, lo);
    TEST_ASSERT_EQUAL_INT16(222, hi);
}

static void test_min_max_null_pointers_return_error(void)
{
    const int16_t v[] = {1, 2};
    int16_t lo = 111, hi = 222;
    TEST_ASSERT_EQUAL_INT(OP_ERR_NULL, min_max_i16(NULL, 2, &lo, &hi));
    TEST_ASSERT_EQUAL_INT(OP_ERR_NULL, min_max_i16(v, 2, NULL, &hi));
    TEST_ASSERT_EQUAL_INT(OP_ERR_NULL, min_max_i16(v, 2, &lo, NULL));
    TEST_ASSERT_EQUAL_INT(OP_ERR_NULL, min_max_i16(NULL, 0, &lo, &hi));
    TEST_ASSERT_EQUAL_INT16(111, lo);
    TEST_ASSERT_EQUAL_INT16(222, hi);
}

/* ---------------------------------------------------------------- divmod */

static void test_divmod_normal(void)
{
    uint32_t q = 0, r = 0;
    TEST_ASSERT_EQUAL_INT(OP_OK, divmod_u32(17, 5, &q, &r));
    TEST_ASSERT_EQUAL_UINT32(3, q);
    TEST_ASSERT_EQUAL_UINT32(2, r);
}

static void test_divmod_exact_and_small_dividend(void)
{
    uint32_t q = 9, r = 9;
    TEST_ASSERT_EQUAL_INT(OP_OK, divmod_u32(20, 4, &q, &r));
    TEST_ASSERT_EQUAL_UINT32(5, q);
    TEST_ASSERT_EQUAL_UINT32(0, r);
    TEST_ASSERT_EQUAL_INT(OP_OK, divmod_u32(3, 1000, &q, &r));
    TEST_ASSERT_EQUAL_UINT32(0, q);
    TEST_ASSERT_EQUAL_UINT32(3, r);
}

static void test_divmod_by_zero_returns_error_and_keeps_outputs(void)
{
    uint32_t q = 7, r = 8;
    TEST_ASSERT_EQUAL_INT(OP_ERR_DIV_ZERO, divmod_u32(10, 0, &q, &r));
    TEST_ASSERT_EQUAL_UINT32(7, q);
    TEST_ASSERT_EQUAL_UINT32(8, r);
}

static void test_divmod_null_outputs_return_error(void)
{
    uint32_t q = 7, r = 8;
    TEST_ASSERT_EQUAL_INT(OP_ERR_NULL, divmod_u32(10, 3, NULL, &r));
    TEST_ASSERT_EQUAL_INT(OP_ERR_NULL, divmod_u32(10, 3, &q, NULL));
    TEST_ASSERT_EQUAL_UINT32(7, q);
    TEST_ASSERT_EQUAL_UINT32(8, r);
}

/* ---------------------------------------------------------------- sort3 */

static void test_sort3_all_orders(void)
{
    const int32_t orders[6][3] = {{1, 2, 3}, {1, 3, 2}, {2, 1, 3}, {2, 3, 1}, {3, 1, 2}, {3, 2, 1}};
    for (int i = 0; i < 6; i++) {
        int32_t a = orders[i][0], b = orders[i][1], c = orders[i][2];
        TEST_ASSERT_EQUAL_INT(OP_OK, sort3_i32(&a, &b, &c));
        TEST_ASSERT_EQUAL_INT32(1, a);
        TEST_ASSERT_EQUAL_INT32(2, b);
        TEST_ASSERT_EQUAL_INT32(3, c);
    }
}

static void test_sort3_with_duplicates_and_negatives(void)
{
    int32_t a = 5, b = -2, c = 5;
    TEST_ASSERT_EQUAL_INT(OP_OK, sort3_i32(&a, &b, &c));
    TEST_ASSERT_EQUAL_INT32(-2, a);
    TEST_ASSERT_EQUAL_INT32(5, b);
    TEST_ASSERT_EQUAL_INT32(5, c);
}

static void test_sort3_null_changes_nothing(void)
{
    int32_t a = 3, b = 2;
    TEST_ASSERT_EQUAL_INT(OP_ERR_NULL, sort3_i32(&a, &b, NULL));
    TEST_ASSERT_EQUAL_INT32(3, a);
    TEST_ASSERT_EQUAL_INT32(2, b);
}

int main(int argc, char **argv)
{
    course_parse_args(argc, argv);
    UNITY_BEGIN();
    RUN_TEST(test_swap_exchanges_values);
    RUN_TEST(test_swap_same_variable_keeps_value);
    RUN_TEST(test_swap_with_null_does_nothing);
    RUN_TEST(test_min_max_of_mixed_values);
    RUN_TEST(test_min_max_of_single_value);
    RUN_TEST(test_min_max_at_the_ends_and_extremes);
    RUN_TEST(test_min_max_reads_only_len_elements);
    RUN_TEST(test_min_max_empty_returns_error_and_keeps_outputs);
    RUN_TEST(test_min_max_null_pointers_return_error);
    RUN_TEST(test_divmod_normal);
    RUN_TEST(test_divmod_exact_and_small_dividend);
    RUN_TEST(test_divmod_by_zero_returns_error_and_keeps_outputs);
    RUN_TEST(test_divmod_null_outputs_return_error);
    RUN_TEST(test_sort3_all_orders);
    RUN_TEST(test_sort3_with_duplicates_and_negatives);
    RUN_TEST(test_sort3_null_changes_nothing);
    return UNITY_END();
}
