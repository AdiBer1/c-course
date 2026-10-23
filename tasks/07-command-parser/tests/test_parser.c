#include <string.h>

#include "course_test.h"
#include "parser.h"

void setUp(void) {}
void tearDown(void) {}

static command_t untouched(void)
{
    command_t c;
    memset(&c, 0x5A, sizeof c);
    c.key[PARSE_KEY_MAX] = '\0'; /* keep it a valid string so failing tests print safely */
    return c;
}

static void assert_unchanged(const command_t *c)
{
    command_t ref = untouched();
    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(&ref, c, sizeof ref, "output must not change on error");
}

static void assert_error(parse_status_t expected, const char *line)
{
    command_t out = untouched();
    TEST_ASSERT_EQUAL_STRING_MESSAGE(parse_status_str(expected), parse_status_str(parse_command(line, &out)), line);
    TEST_ASSERT_EQUAL_INT_MESSAGE(expected, parse_command(line, &out), line);
    assert_unchanged(&out);
}

/* ---------------------------------------------------------------- valid lines */

static void test_parse_simple_command(void)
{
    command_t c = untouched();
    TEST_ASSERT_EQUAL_INT(PARSE_OK, parse_command("LED=1", &c));
    TEST_ASSERT_EQUAL_STRING("LED", c.key);
    TEST_ASSERT_EQUAL_INT32(1, c.value);
}

static void test_parse_negative_value_and_underscore_key(void)
{
    command_t c = untouched();
    TEST_ASSERT_EQUAL_INT(PARSE_OK, parse_command("MOTOR_SPD=-250", &c));
    TEST_ASSERT_EQUAL_STRING("MOTOR_SPD", c.key);
    TEST_ASSERT_EQUAL_INT32(-250, c.value);
}

static void test_parse_key_with_digits(void)
{
    command_t c = untouched();
    TEST_ASSERT_EQUAL_INT(PARSE_OK, parse_command("CH2=30", &c));
    TEST_ASSERT_EQUAL_STRING("CH2", c.key);
    TEST_ASSERT_EQUAL_INT32(30, c.value);
}

static void test_parse_strips_line_endings(void)
{
    command_t c = untouched();
    TEST_ASSERT_EQUAL_INT(PARSE_OK, parse_command("LED=0\n", &c));
    TEST_ASSERT_EQUAL_INT32(0, c.value);
    TEST_ASSERT_EQUAL_INT(PARSE_OK, parse_command("TIMEOUT=30000\r\n", &c));
    TEST_ASSERT_EQUAL_STRING("TIMEOUT", c.key);
    TEST_ASSERT_EQUAL_INT32(30000, c.value);
}

static void test_parse_key_of_maximum_length(void)
{
    command_t c = untouched();
    TEST_ASSERT_EQUAL_INT(PARSE_OK, parse_command("ABCDEFGHIJ=5", &c));
    TEST_ASSERT_EQUAL_STRING("ABCDEFGHIJ", c.key);
}

static void test_parse_value_range_limits(void)
{
    command_t c = untouched();
    TEST_ASSERT_EQUAL_INT(PARSE_OK, parse_command("X=1000000", &c));
    TEST_ASSERT_EQUAL_INT32(1000000, c.value);
    TEST_ASSERT_EQUAL_INT(PARSE_OK, parse_command("X=-1000000", &c));
    TEST_ASSERT_EQUAL_INT32(-1000000, c.value);
}

static void test_parse_leading_zeros_and_minus_zero(void)
{
    command_t c = untouched();
    TEST_ASSERT_EQUAL_INT(PARSE_OK, parse_command("A=007", &c));
    TEST_ASSERT_EQUAL_INT32(7, c.value);
    TEST_ASSERT_EQUAL_INT(PARSE_OK, parse_command("A=-0", &c));
    TEST_ASSERT_EQUAL_INT32(0, c.value);
}

/* ---------------------------------------------------------------- errors */

static void test_null_arguments(void)
{
    command_t c = untouched();
    TEST_ASSERT_EQUAL_INT(PARSE_ERR_NULL, parse_command(NULL, &c));
    assert_unchanged(&c);
    TEST_ASSERT_EQUAL_INT(PARSE_ERR_NULL, parse_command("LED=1", NULL));
}

static void test_empty_lines(void)
{
    assert_error(PARSE_ERR_EMPTY, "");
    assert_error(PARSE_ERR_EMPTY, "\n");
    assert_error(PARSE_ERR_EMPTY, "\r\n");
}

static void test_missing_equals(void)
{
    assert_error(PARSE_ERR_NO_EQUALS, "LED1");
    assert_error(PARSE_ERR_NO_EQUALS, "LED\n");
    assert_error(PARSE_ERR_NO_EQUALS, "ABCDEFGHIJKLMNOP");
}

static void test_bad_keys(void)
{
    assert_error(PARSE_ERR_BAD_KEY, "=5");
    assert_error(PARSE_ERR_BAD_KEY, "1LED=5");
    assert_error(PARSE_ERR_BAD_KEY, "_LED=5");
    assert_error(PARSE_ERR_BAD_KEY, "led=1");
    assert_error(PARSE_ERR_BAD_KEY, "LED 1");
    assert_error(PARSE_ERR_BAD_KEY, "LED =1");
    assert_error(PARSE_ERR_BAD_KEY, " LED=1");
}

static void test_key_too_long(void)
{
    assert_error(PARSE_ERR_KEY_TOO_LONG, "ABCDEFGHIJK=1");
    assert_error(PARSE_ERR_KEY_TOO_LONG, "VERY_LONG_KEY_NAME=1");
}

static void test_bad_values(void)
{
    assert_error(PARSE_ERR_BAD_VALUE, "LED=");
    assert_error(PARSE_ERR_BAD_VALUE, "LED=-");
    assert_error(PARSE_ERR_BAD_VALUE, "LED=5x");
    assert_error(PARSE_ERR_BAD_VALUE, "LED=x5");
    assert_error(PARSE_ERR_BAD_VALUE, "LED= 5");
    assert_error(PARSE_ERR_BAD_VALUE, "LED=+5");
    assert_error(PARSE_ERR_BAD_VALUE, "LED=1=2");
    assert_error(PARSE_ERR_BAD_VALUE, "LED=1\r");
    assert_error(PARSE_ERR_BAD_VALUE, "LED=1\n\n");
    assert_error(PARSE_ERR_BAD_VALUE, "LED=--1");
}

static void test_value_out_of_range(void)
{
    assert_error(PARSE_ERR_RANGE, "X=1000001");
    assert_error(PARSE_ERR_RANGE, "X=-1000001");
}

static void test_huge_values_do_not_overflow(void)
{
    /* These would overflow int32_t if accumulated naively. */
    assert_error(PARSE_ERR_RANGE, "X=99999999999999999999");
    assert_error(PARSE_ERR_RANGE, "X=-4294967296");
    assert_error(PARSE_ERR_RANGE, "X=2147483648");
}

static void test_syntax_error_wins_over_range_error(void)
{
    assert_error(PARSE_ERR_BAD_VALUE, "X=99999999x");
}

/* ---------------------------------------------------------------- status names */

static void test_status_names(void)
{
    TEST_ASSERT_EQUAL_STRING("PARSE_OK", parse_status_str(PARSE_OK));
    TEST_ASSERT_EQUAL_STRING("PARSE_ERR_NULL", parse_status_str(PARSE_ERR_NULL));
    TEST_ASSERT_EQUAL_STRING("PARSE_ERR_EMPTY", parse_status_str(PARSE_ERR_EMPTY));
    TEST_ASSERT_EQUAL_STRING("PARSE_ERR_NO_EQUALS", parse_status_str(PARSE_ERR_NO_EQUALS));
    TEST_ASSERT_EQUAL_STRING("PARSE_ERR_BAD_KEY", parse_status_str(PARSE_ERR_BAD_KEY));
    TEST_ASSERT_EQUAL_STRING("PARSE_ERR_KEY_TOO_LONG", parse_status_str(PARSE_ERR_KEY_TOO_LONG));
    TEST_ASSERT_EQUAL_STRING("PARSE_ERR_BAD_VALUE", parse_status_str(PARSE_ERR_BAD_VALUE));
    TEST_ASSERT_EQUAL_STRING("PARSE_ERR_RANGE", parse_status_str(PARSE_ERR_RANGE));
    TEST_ASSERT_EQUAL_STRING("UNKNOWN", parse_status_str((parse_status_t)99));
}

int main(int argc, char **argv)
{
    course_parse_args(argc, argv);
    UNITY_BEGIN();
    RUN_TEST(test_parse_simple_command);
    RUN_TEST(test_parse_negative_value_and_underscore_key);
    RUN_TEST(test_parse_key_with_digits);
    RUN_TEST(test_parse_strips_line_endings);
    RUN_TEST(test_parse_key_of_maximum_length);
    RUN_TEST(test_parse_value_range_limits);
    RUN_TEST(test_parse_leading_zeros_and_minus_zero);
    RUN_TEST(test_null_arguments);
    RUN_TEST(test_empty_lines);
    RUN_TEST(test_missing_equals);
    RUN_TEST(test_bad_keys);
    RUN_TEST(test_key_too_long);
    RUN_TEST(test_bad_values);
    RUN_TEST(test_value_out_of_range);
    RUN_TEST(test_huge_values_do_not_overflow);
    RUN_TEST(test_syntax_error_wins_over_range_error);
    RUN_TEST(test_status_names);
    return UNITY_END();
}
