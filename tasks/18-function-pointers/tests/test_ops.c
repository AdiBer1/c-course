#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#include "course_test.h"
#include "ops.h"

void setUp(void) {}
void tearDown(void) {}

/* ---------------------------------------------------------------- part 1 */

static int32_t times_ten(int32_t x)
{
    return x * 10;
}

static int32_t negate(int32_t x)
{
    return -x;
}

static void test_basic_operations(void)
{
    TEST_ASSERT_EQUAL_INT32(5, op_add(2, 3));
    TEST_ASSERT_EQUAL_INT32(-1, op_sub(2, 3));
    TEST_ASSERT_EQUAL_INT32(-12, op_mul(4, -3));
    TEST_ASSERT_EQUAL_INT32(7, op_max(7, -9));
    TEST_ASSERT_EQUAL_INT32(-2, op_max(-5, -2));
}

static void test_apply_op_calls_the_given_function(void)
{
    TEST_ASSERT_EQUAL_INT32(5, apply_op(op_add, 2, 3, -1));
    TEST_ASSERT_EQUAL_INT32(6, apply_op(op_mul, 2, 3, -1));
    TEST_ASSERT_EQUAL_INT32(3, apply_op(op_max, 2, 3, -1));
}

static void test_apply_op_with_null_returns_fallback(void)
{
    TEST_ASSERT_EQUAL_INT32(-1, apply_op(NULL, 2, 3, -1));
    TEST_ASSERT_EQUAL_INT32(77, apply_op(NULL, 0, 0, 77));
}

static void test_map_applies_function_to_each_element(void)
{
    int32_t v[] = {1, -2, 3};
    map_i32(v, 3, times_ten);
    TEST_ASSERT_EQUAL_INT32(10, v[0]);
    TEST_ASSERT_EQUAL_INT32(-20, v[1]);
    TEST_ASSERT_EQUAL_INT32(30, v[2]);
    map_i32(v, 2, negate);
    TEST_ASSERT_EQUAL_INT32(-10, v[0]);
    TEST_ASSERT_EQUAL_INT32(20, v[1]);
    TEST_ASSERT_EQUAL_INT32(30, v[2]);
}

static void test_map_with_null_does_nothing(void)
{
    int32_t v[] = {4};
    map_i32(v, 1, NULL);
    map_i32(NULL, 1, negate);
    TEST_ASSERT_EQUAL_INT32(4, v[0]);
}

/* ---------------------------------------------------------------- part 2 */

static void test_table_has_four_entries_with_functions(void)
{
    TEST_ASSERT_EQUAL_size_t(4, OP_TABLE_LEN);
    for (size_t i = 0; i < OP_TABLE_LEN; i++) {
        TEST_ASSERT_NOT_NULL(OP_TABLE[i].fn);
    }
}

static void test_lookup_finds_each_operation(void)
{
    TEST_ASSERT_EQUAL_PTR(op_add, op_lookup('+'));
    TEST_ASSERT_EQUAL_PTR(op_sub, op_lookup('-'));
    TEST_ASSERT_EQUAL_PTR(op_mul, op_lookup('*'));
    TEST_ASSERT_EQUAL_PTR(op_max, op_lookup('M'));
}

static void test_lookup_unknown_symbol_returns_null(void)
{
    TEST_ASSERT_NULL(op_lookup('/'));
    TEST_ASSERT_NULL(op_lookup('m'));
    TEST_ASSERT_NULL(op_lookup('\0'));
}

static void test_calc_through_the_table(void)
{
    int32_t r = 0;
    TEST_ASSERT_TRUE(calc('+', 40, 2, &r));
    TEST_ASSERT_EQUAL_INT32(42, r);
    TEST_ASSERT_TRUE(calc('*', -6, 7, &r));
    TEST_ASSERT_EQUAL_INT32(-42, r);
    TEST_ASSERT_TRUE(calc('M', 3, 9, &r));
    TEST_ASSERT_EQUAL_INT32(9, r);
}

static void test_calc_unknown_symbol_or_null_result(void)
{
    int32_t r = 123;
    TEST_ASSERT_FALSE(calc('%', 1, 2, &r));
    TEST_ASSERT_EQUAL_INT32(123, r);
    TEST_ASSERT_FALSE(calc('+', 1, 2, NULL));
}

/* ---------------------------------------------------------------- part 3 */

static int sign(int x)
{
    return (x > 0) - (x < 0);
}

static void test_comparators(void)
{
    TEST_ASSERT_EQUAL_INT(-1, sign(cmp_ascending(1, 2)));
    TEST_ASSERT_EQUAL_INT(1, sign(cmp_ascending(2, 1)));
    TEST_ASSERT_EQUAL_INT(0, sign(cmp_ascending(5, 5)));
    TEST_ASSERT_EQUAL_INT(1, sign(cmp_descending(1, 2)));
    TEST_ASSERT_EQUAL_INT(-1, sign(cmp_descending(2, 1)));
    TEST_ASSERT_EQUAL_INT(-1, sign(cmp_abs(-2, 3)));
    TEST_ASSERT_EQUAL_INT(1, sign(cmp_abs(-4, 3)));
    TEST_ASSERT_EQUAL_INT(-1, sign(cmp_abs(-3, 3)));
    TEST_ASSERT_EQUAL_INT(0, sign(cmp_abs(-3, -3)));
}

static void test_comparators_do_not_overflow(void)
{
    /* "return a - b;" overflows here */
    TEST_ASSERT_EQUAL_INT(-1, sign(cmp_ascending(INT32_MIN, 1)));
    TEST_ASSERT_EQUAL_INT(1, sign(cmp_ascending(INT32_MAX, -1)));
    TEST_ASSERT_EQUAL_INT(1, sign(cmp_descending(INT32_MIN, INT32_MAX)));
}

static void test_sort_ascending(void)
{
    int32_t v[] = {5, -1, 3, 3, 0, 9, -7};
    const int32_t expected[] = {-7, -1, 0, 3, 3, 5, 9};
    sort_i32(v, 7, cmp_ascending);
    TEST_ASSERT_EQUAL_INT32_ARRAY(expected, v, 7);
}

static void test_sort_descending(void)
{
    int32_t v[] = {5, -1, 3, 0, 9, -7};
    const int32_t expected[] = {9, 5, 3, 0, -1, -7};
    sort_i32(v, 6, cmp_descending);
    TEST_ASSERT_EQUAL_INT32_ARRAY(expected, v, 6);
}

static void test_sort_by_absolute_value(void)
{
    int32_t v[] = {-10, 4, -3, 3, 1, -4};
    const int32_t expected[] = {1, -3, 3, -4, 4, -10};
    sort_i32(v, 6, cmp_abs);
    TEST_ASSERT_EQUAL_INT32_ARRAY(expected, v, 6);
}

static int comparisons;

static int counting_cmp(int32_t a, int32_t b)
{
    comparisons++;
    return cmp_ascending(a, b);
}

static void test_sort_uses_the_comparator(void)
{
    int32_t v[] = {3, 1, 2};
    comparisons = 0;
    sort_i32(v, 3, counting_cmp);
    TEST_ASSERT_TRUE(comparisons > 0);
    TEST_ASSERT_EQUAL_INT32(1, v[0]);
    TEST_ASSERT_EQUAL_INT32(3, v[2]);
}

static void test_sort_edge_cases(void)
{
    int32_t one[] = {42};
    int32_t v[] = {2, 1};
    sort_i32(one, 1, cmp_ascending);
    TEST_ASSERT_EQUAL_INT32(42, one[0]);
    sort_i32(v, 0, cmp_ascending);
    TEST_ASSERT_EQUAL_INT32(2, v[0]);
    sort_i32(NULL, 5, cmp_ascending);
    sort_i32(v, 2, NULL);
    TEST_ASSERT_EQUAL_INT32(2, v[0]);
}

int main(int argc, char **argv)
{
    course_parse_args(argc, argv);
    UNITY_BEGIN();
    RUN_TEST(test_basic_operations);
    RUN_TEST(test_apply_op_calls_the_given_function);
    RUN_TEST(test_apply_op_with_null_returns_fallback);
    RUN_TEST(test_map_applies_function_to_each_element);
    RUN_TEST(test_map_with_null_does_nothing);
    RUN_TEST(test_table_has_four_entries_with_functions);
    RUN_TEST(test_lookup_finds_each_operation);
    RUN_TEST(test_lookup_unknown_symbol_returns_null);
    RUN_TEST(test_calc_through_the_table);
    RUN_TEST(test_calc_unknown_symbol_or_null_result);
    RUN_TEST(test_comparators);
    RUN_TEST(test_comparators_do_not_overflow);
    RUN_TEST(test_sort_ascending);
    RUN_TEST(test_sort_descending);
    RUN_TEST(test_sort_by_absolute_value);
    RUN_TEST(test_sort_uses_the_comparator);
    RUN_TEST(test_sort_edge_cases);
    return UNITY_END();
}
