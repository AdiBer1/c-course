#include <stddef.h>
#include <string.h> /* the TESTS may use the real functions to check yours */

#include "course_test.h"
#include "my_string.h"

void setUp(void) {}
void tearDown(void) {}

/* ---------------------------------------------------------------- strlen */

static void test_strlen_of_normal_string(void)
{
    TEST_ASSERT_EQUAL_size_t(5, my_strlen("hello"));
}

static void test_strlen_of_empty_string_and_null(void)
{
    TEST_ASSERT_EQUAL_size_t(0, my_strlen(""));
    TEST_ASSERT_EQUAL_size_t(0, my_strlen(NULL));
}

static void test_strlen_stops_at_first_nul(void)
{
    const char s[] = {'a', 'b', '\0', 'c', 'd', '\0'};
    TEST_ASSERT_EQUAL_size_t(2, my_strlen(s));
}

static void test_strlen_of_long_string(void)
{
    char s[300];
    memset(s, 'x', sizeof s - 1);
    s[sizeof s - 1] = '\0';
    TEST_ASSERT_EQUAL_size_t(299, my_strlen(s));
}

/* ---------------------------------------------------------------- strlcpy */

static void test_strlcpy_fits(void)
{
    char dst[10];
    memset(dst, '#', sizeof dst);
    TEST_ASSERT_EQUAL_size_t(3, my_strlcpy(dst, "abc", sizeof dst));
    TEST_ASSERT_EQUAL_STRING("abc", dst);
}

static void test_strlcpy_exactly_fills_buffer(void)
{
    char dst[4];
    TEST_ASSERT_EQUAL_size_t(3, my_strlcpy(dst, "abc", sizeof dst));
    TEST_ASSERT_EQUAL_STRING("abc", dst);
}

static void test_strlcpy_truncates_and_terminates(void)
{
    char dst[4];
    TEST_ASSERT_EQUAL_size_t(6, my_strlcpy(dst, "abcdef", sizeof dst));
    TEST_ASSERT_EQUAL_STRING("abc", dst);
}

static void test_strlcpy_never_writes_past_the_buffer(void)
{
    char area[8];
    memset(area, '#', sizeof area);
    /* only the first 4 bytes belong to the "buffer" */
    my_strlcpy(area, "abcdefgh", 4);
    TEST_ASSERT_EQUAL_CHAR('\0', area[3]);
    TEST_ASSERT_EQUAL_CHAR('#', area[4]);
    TEST_ASSERT_EQUAL_CHAR('#', area[7]);
}

static void test_strlcpy_size_one_gives_empty_string(void)
{
    char dst[2] = {'#', '#'};
    TEST_ASSERT_EQUAL_size_t(5, my_strlcpy(dst, "hello", 1));
    TEST_ASSERT_EQUAL_CHAR('\0', dst[0]);
    TEST_ASSERT_EQUAL_CHAR('#', dst[1]);
}

static void test_strlcpy_size_zero_writes_nothing(void)
{
    char dst[1] = {'#'};
    TEST_ASSERT_EQUAL_size_t(5, my_strlcpy(dst, "hello", 0));
    TEST_ASSERT_EQUAL_CHAR('#', dst[0]);
}

static void test_strlcpy_null_arguments(void)
{
    char dst[4] = {'#', '#', '#', '#'};
    TEST_ASSERT_EQUAL_size_t(0, my_strlcpy(dst, NULL, sizeof dst));
    TEST_ASSERT_EQUAL_STRING("", dst);
    TEST_ASSERT_EQUAL_size_t(5, my_strlcpy(NULL, "hello", 10));
}

/* ---------------------------------------------------------------- strcmp */

static int sign(int x)
{
    return (x > 0) - (x < 0);
}

static void test_strcmp_equal_strings(void)
{
    TEST_ASSERT_EQUAL_INT(0, my_strcmp("LED", "LED"));
    TEST_ASSERT_EQUAL_INT(0, my_strcmp("", ""));
}

static void test_strcmp_order(void)
{
    TEST_ASSERT_EQUAL_INT(-1, sign(my_strcmp("apple", "banana")));
    TEST_ASSERT_EQUAL_INT(1, sign(my_strcmp("banana", "apple")));
    TEST_ASSERT_EQUAL_INT(-1, sign(my_strcmp("LED", "led")));
}

static void test_strcmp_prefix_sorts_first(void)
{
    TEST_ASSERT_EQUAL_INT(-1, sign(my_strcmp("abc", "abcd")));
    TEST_ASSERT_EQUAL_INT(1, sign(my_strcmp("abcd", "abc")));
    TEST_ASSERT_EQUAL_INT(-1, sign(my_strcmp("", "a")));
}

static void test_strcmp_compares_as_unsigned_char(void)
{
    /* 0xE9 is 233 as unsigned char, but -23 as a signed char */
    TEST_ASSERT_EQUAL_INT(1, sign(my_strcmp("\xE9", "e")));
    TEST_ASSERT_EQUAL_INT(-1, sign(my_strcmp("e", "\xE9")));
}

static void test_strcmp_null_counts_as_empty(void)
{
    TEST_ASSERT_EQUAL_INT(0, my_strcmp(NULL, ""));
    TEST_ASSERT_EQUAL_INT(0, my_strcmp(NULL, NULL));
    TEST_ASSERT_EQUAL_INT(-1, sign(my_strcmp(NULL, "x")));
}

static void test_strcmp_agrees_with_library(void)
{
    const char *words[] = {"", "a", "ab", "abc", "b", "B", "zz", "z", "LED=1", "LED=0"};
    const size_t n = sizeof words / sizeof words[0];
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            TEST_ASSERT_EQUAL_INT(sign(strcmp(words[i], words[j])), sign(my_strcmp(words[i], words[j])));
        }
    }
}

int main(int argc, char **argv)
{
    course_parse_args(argc, argv);
    UNITY_BEGIN();
    RUN_TEST(test_strlen_of_normal_string);
    RUN_TEST(test_strlen_of_empty_string_and_null);
    RUN_TEST(test_strlen_stops_at_first_nul);
    RUN_TEST(test_strlen_of_long_string);
    RUN_TEST(test_strlcpy_fits);
    RUN_TEST(test_strlcpy_exactly_fills_buffer);
    RUN_TEST(test_strlcpy_truncates_and_terminates);
    RUN_TEST(test_strlcpy_never_writes_past_the_buffer);
    RUN_TEST(test_strlcpy_size_one_gives_empty_string);
    RUN_TEST(test_strlcpy_size_zero_writes_nothing);
    RUN_TEST(test_strlcpy_null_arguments);
    RUN_TEST(test_strcmp_equal_strings);
    RUN_TEST(test_strcmp_order);
    RUN_TEST(test_strcmp_prefix_sorts_first);
    RUN_TEST(test_strcmp_compares_as_unsigned_char);
    RUN_TEST(test_strcmp_null_counts_as_empty);
    RUN_TEST(test_strcmp_agrees_with_library);
    return UNITY_END();
}
