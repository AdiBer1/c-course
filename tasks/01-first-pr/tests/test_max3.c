#include <limits.h>

#include "course_test.h"
#include "max3.h"

void setUp(void) {}
void tearDown(void) {}

static void test_largest_is_first(void)
{
    TEST_ASSERT_EQUAL_INT(9, max3(9, 2, 5));
}

static void test_largest_is_second(void)
{
    TEST_ASSERT_EQUAL_INT(9, max3(2, 9, 5));
}

static void test_largest_is_third(void)
{
    TEST_ASSERT_EQUAL_INT(9, max3(2, 5, 9));
}

static void test_all_negative(void)
{
    TEST_ASSERT_EQUAL_INT(-5, max3(-5, -9, -7));
}

static void test_two_equal_largest(void)
{
    TEST_ASSERT_EQUAL_INT(4, max3(4, 4, 1));
    TEST_ASSERT_EQUAL_INT(4, max3(1, 4, 4));
}

static void test_all_equal(void)
{
    TEST_ASSERT_EQUAL_INT(-3, max3(-3, -3, -3));
}

static void test_extreme_values(void)
{
    TEST_ASSERT_EQUAL_INT(INT_MAX, max3(INT_MIN, INT_MAX, 0));
    TEST_ASSERT_EQUAL_INT(INT_MIN, max3(INT_MIN, INT_MIN, INT_MIN));
}

int main(int argc, char **argv)
{
    course_parse_args(argc, argv);
    UNITY_BEGIN();
    RUN_TEST(test_largest_is_first);
    RUN_TEST(test_largest_is_second);
    RUN_TEST(test_largest_is_third);
    RUN_TEST(test_all_negative);
    RUN_TEST(test_two_equal_largest);
    RUN_TEST(test_all_equal);
    RUN_TEST(test_extreme_values);
    return UNITY_END();
}
