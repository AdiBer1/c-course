#include "command.h"
#include "course_test.h"

void setUp(void) {}
void tearDown(void) {}

/* ---------------------------------------------------------------- classify */

static void test_classify_led_on_and_off(void)
{
    TEST_ASSERT_EQUAL_INT(CMD_LED_ON, command_classify('1'));
    TEST_ASSERT_EQUAL_INT(CMD_LED_OFF, command_classify('0'));
}

static void test_classify_accepts_both_cases(void)
{
    TEST_ASSERT_EQUAL_INT(CMD_LED_TOGGLE, command_classify('t'));
    TEST_ASSERT_EQUAL_INT(CMD_LED_TOGGLE, command_classify('T'));
    TEST_ASSERT_EQUAL_INT(CMD_STATUS, command_classify('s'));
    TEST_ASSERT_EQUAL_INT(CMD_STATUS, command_classify('S'));
    TEST_ASSERT_EQUAL_INT(CMD_RESET, command_classify('r'));
    TEST_ASSERT_EQUAL_INT(CMD_RESET, command_classify('R'));
}

static void test_classify_whitespace_is_none(void)
{
    TEST_ASSERT_EQUAL_INT(CMD_NONE, command_classify(' '));
    TEST_ASSERT_EQUAL_INT(CMD_NONE, command_classify('\r'));
    TEST_ASSERT_EQUAL_INT(CMD_NONE, command_classify('\n'));
    TEST_ASSERT_EQUAL_INT(CMD_NONE, command_classify('\t'));
}

static void test_classify_other_characters_are_unknown(void)
{
    TEST_ASSERT_EQUAL_INT(CMD_UNKNOWN, command_classify('2'));
    TEST_ASSERT_EQUAL_INT(CMD_UNKNOWN, command_classify('x'));
    TEST_ASSERT_EQUAL_INT(CMD_UNKNOWN, command_classify('?'));
    TEST_ASSERT_EQUAL_INT(CMD_UNKNOWN, command_classify('\0'));
}

static void test_classify_every_char_gives_a_valid_command(void)
{
    for (int c = -128; c <= 127; c++) {
        command_t cmd = command_classify((char)c);
        TEST_ASSERT_TRUE((int)cmd >= (int)CMD_NONE && (int)cmd < (int)CMD_COUNT);
    }
}

/* ---------------------------------------------------------------- LED state */

static void test_led_on_and_off_commands(void)
{
    TEST_ASSERT_EQUAL_INT(LED_ON, led_next_state(LED_OFF, CMD_LED_ON));
    TEST_ASSERT_EQUAL_INT(LED_ON, led_next_state(LED_ON, CMD_LED_ON));
    TEST_ASSERT_EQUAL_INT(LED_OFF, led_next_state(LED_ON, CMD_LED_OFF));
}

static void test_led_toggle_flips_state(void)
{
    TEST_ASSERT_EQUAL_INT(LED_ON, led_next_state(LED_OFF, CMD_LED_TOGGLE));
    TEST_ASSERT_EQUAL_INT(LED_OFF, led_next_state(LED_ON, CMD_LED_TOGGLE));
}

static void test_led_reset_turns_led_off(void)
{
    TEST_ASSERT_EQUAL_INT(LED_OFF, led_next_state(LED_ON, CMD_RESET));
}

static void test_led_other_commands_keep_state(void)
{
    TEST_ASSERT_EQUAL_INT(LED_ON, led_next_state(LED_ON, CMD_STATUS));
    TEST_ASSERT_EQUAL_INT(LED_ON, led_next_state(LED_ON, CMD_NONE));
    TEST_ASSERT_EQUAL_INT(LED_ON, led_next_state(LED_ON, CMD_UNKNOWN));
    TEST_ASSERT_EQUAL_INT(LED_ON, led_next_state(LED_ON, CMD_COUNT));
}

static void test_led_invalid_current_state_counts_as_off(void)
{
    TEST_ASSERT_EQUAL_INT(LED_OFF, led_next_state((led_state_t)7, CMD_STATUS));
    TEST_ASSERT_EQUAL_INT(LED_ON, led_next_state((led_state_t)7, CMD_LED_TOGGLE));
}

/* ---------------------------------------------------------------- names */

static void test_names_of_all_commands(void)
{
    TEST_ASSERT_EQUAL_STRING("NONE", command_name(CMD_NONE));
    TEST_ASSERT_EQUAL_STRING("LED_ON", command_name(CMD_LED_ON));
    TEST_ASSERT_EQUAL_STRING("LED_OFF", command_name(CMD_LED_OFF));
    TEST_ASSERT_EQUAL_STRING("LED_TOGGLE", command_name(CMD_LED_TOGGLE));
    TEST_ASSERT_EQUAL_STRING("STATUS", command_name(CMD_STATUS));
    TEST_ASSERT_EQUAL_STRING("RESET", command_name(CMD_RESET));
    TEST_ASSERT_EQUAL_STRING("UNKNOWN", command_name(CMD_UNKNOWN));
}

static void test_name_of_out_of_range_value_is_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("INVALID", command_name(CMD_COUNT));
    TEST_ASSERT_EQUAL_STRING("INVALID", command_name((command_t)42));
    TEST_ASSERT_EQUAL_STRING("INVALID", command_name((command_t)-1));
}

static void test_classify_then_name_round_trip(void)
{
    TEST_ASSERT_EQUAL_STRING("LED_TOGGLE", command_name(command_classify('T')));
    TEST_ASSERT_EQUAL_STRING("UNKNOWN", command_name(command_classify('#')));
}

int main(int argc, char **argv)
{
    course_parse_args(argc, argv);
    UNITY_BEGIN();
    RUN_TEST(test_classify_led_on_and_off);
    RUN_TEST(test_classify_accepts_both_cases);
    RUN_TEST(test_classify_whitespace_is_none);
    RUN_TEST(test_classify_other_characters_are_unknown);
    RUN_TEST(test_classify_every_char_gives_a_valid_command);
    RUN_TEST(test_led_on_and_off_commands);
    RUN_TEST(test_led_toggle_flips_state);
    RUN_TEST(test_led_reset_turns_led_off);
    RUN_TEST(test_led_other_commands_keep_state);
    RUN_TEST(test_led_invalid_current_state_counts_as_off);
    RUN_TEST(test_names_of_all_commands);
    RUN_TEST(test_name_of_out_of_range_value_is_invalid);
    RUN_TEST(test_classify_then_name_round_trip);
    return UNITY_END();
}
