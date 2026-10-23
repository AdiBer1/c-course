#include <stdbool.h>
#include <stddef.h>

#include "course_test.h"
#include "debounce.h"

static debounce_t btn;

void setUp(void)
{
    debounce_init(&btn, 5);
}

void tearDown(void) {}

/* Feed `n` ticks of the same raw value; return how many events of `kind` happened. */
static unsigned feed(bool raw, unsigned n, btn_event_t kind)
{
    unsigned events = 0;
    for (unsigned i = 0; i < n; i++) {
        if (debounce_update(&btn, raw) == kind) {
            events++;
        }
    }
    return events;
}

static void test_starts_released(void)
{
    TEST_ASSERT_EQUAL_INT(BTN_RELEASED, debounce_state(&btn));
    TEST_ASSERT_EQUAL_UINT32(0, debounce_press_count(&btn));
}

static void test_steady_release_gives_no_events(void)
{
    for (int i = 0; i < 100; i++) {
        TEST_ASSERT_EQUAL_INT(BTN_EVENT_NONE, debounce_update(&btn, false));
    }
}

static void test_press_is_reported_exactly_on_threshold_tick(void)
{
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_INT(BTN_EVENT_NONE, debounce_update(&btn, true));
        TEST_ASSERT_EQUAL_INT(BTN_RELEASED, debounce_state(&btn));
    }
    TEST_ASSERT_EQUAL_INT(BTN_EVENT_PRESS, debounce_update(&btn, true));
    TEST_ASSERT_EQUAL_INT(BTN_PRESSED, debounce_state(&btn));
}

static void test_holding_the_button_reports_one_press_only(void)
{
    TEST_ASSERT_EQUAL_UINT(1, feed(true, 200, BTN_EVENT_PRESS));
    TEST_ASSERT_EQUAL_UINT32(1, debounce_press_count(&btn));
}

static void test_release_after_press(void)
{
    feed(true, 10, BTN_EVENT_PRESS);
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_INT(BTN_EVENT_NONE, debounce_update(&btn, false));
    }
    TEST_ASSERT_EQUAL_INT(BTN_EVENT_RELEASE, debounce_update(&btn, false));
    TEST_ASSERT_EQUAL_INT(BTN_RELEASED, debounce_state(&btn));
}

static void test_short_glitch_is_ignored(void)
{
    feed(true, 4, BTN_EVENT_PRESS); /* 4 < threshold */
    TEST_ASSERT_EQUAL_INT(BTN_EVENT_NONE, debounce_update(&btn, false));
    /* the counter must start again from 0 */
    TEST_ASSERT_EQUAL_UINT(0, feed(true, 4, BTN_EVENT_PRESS));
    TEST_ASSERT_EQUAL_INT(BTN_RELEASED, debounce_state(&btn));
}

static void test_bouncing_press_counts_as_one_press(void)
{
    /* contact bounces, then settles closed */
    const bool bounce[] = {1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1};
    unsigned presses = 0;
    for (size_t i = 0; i < sizeof bounce / sizeof bounce[0]; i++) {
        presses += debounce_update(&btn, bounce[i]) == BTN_EVENT_PRESS;
    }
    presses += feed(true, 20, BTN_EVENT_PRESS);
    TEST_ASSERT_EQUAL_UINT(1, presses);
}

static void test_bouncing_release_counts_as_one_release(void)
{
    feed(true, 10, BTN_EVENT_PRESS);
    const bool bounce[] = {0, 1, 0, 0, 1, 0, 1, 0, 0, 0};
    unsigned releases = 0;
    for (size_t i = 0; i < sizeof bounce / sizeof bounce[0]; i++) {
        releases += debounce_update(&btn, bounce[i]) == BTN_EVENT_RELEASE;
    }
    releases += feed(false, 20, BTN_EVENT_RELEASE);
    TEST_ASSERT_EQUAL_UINT(1, releases);
}

static void test_press_count_over_several_presses(void)
{
    for (int i = 0; i < 3; i++) {
        feed(true, 10, BTN_EVENT_PRESS);
        feed(false, 10, BTN_EVENT_RELEASE);
    }
    TEST_ASSERT_EQUAL_UINT32(3, debounce_press_count(&btn));
}

static void test_threshold_one_reacts_immediately(void)
{
    debounce_init(&btn, 1);
    TEST_ASSERT_EQUAL_INT(BTN_EVENT_PRESS, debounce_update(&btn, true));
    TEST_ASSERT_EQUAL_INT(BTN_EVENT_RELEASE, debounce_update(&btn, false));
}

static void test_threshold_zero_counts_as_one(void)
{
    debounce_init(&btn, 0);
    TEST_ASSERT_EQUAL_INT(BTN_EVENT_PRESS, debounce_update(&btn, true));
}

static void test_large_threshold(void)
{
    debounce_init(&btn, 255);
    TEST_ASSERT_EQUAL_UINT(0, feed(true, 254, BTN_EVENT_PRESS));
    TEST_ASSERT_EQUAL_INT(BTN_EVENT_PRESS, debounce_update(&btn, true));
}

static void test_init_resets_everything(void)
{
    feed(true, 10, BTN_EVENT_PRESS);
    debounce_init(&btn, 5);
    TEST_ASSERT_EQUAL_INT(BTN_RELEASED, debounce_state(&btn));
    TEST_ASSERT_EQUAL_UINT32(0, debounce_press_count(&btn));
    TEST_ASSERT_EQUAL_UINT(0, feed(true, 4, BTN_EVENT_PRESS));
}

static void test_null_is_handled(void)
{
    debounce_init(NULL, 5);
    TEST_ASSERT_EQUAL_INT(BTN_EVENT_NONE, debounce_update(NULL, true));
    TEST_ASSERT_EQUAL_INT(BTN_RELEASED, debounce_state(NULL));
    TEST_ASSERT_EQUAL_UINT32(0, debounce_press_count(NULL));
}

static void test_two_buttons_are_independent(void)
{
    debounce_t other;
    debounce_init(&other, 2);
    feed(true, 10, BTN_EVENT_PRESS);
    TEST_ASSERT_EQUAL_INT(BTN_RELEASED, debounce_state(&other));
    debounce_update(&other, true);
    TEST_ASSERT_EQUAL_INT(BTN_EVENT_PRESS, debounce_update(&other, true));
    TEST_ASSERT_EQUAL_UINT32(1, debounce_press_count(&btn));
}

int main(int argc, char **argv)
{
    course_parse_args(argc, argv);
    UNITY_BEGIN();
    RUN_TEST(test_starts_released);
    RUN_TEST(test_steady_release_gives_no_events);
    RUN_TEST(test_press_is_reported_exactly_on_threshold_tick);
    RUN_TEST(test_holding_the_button_reports_one_press_only);
    RUN_TEST(test_release_after_press);
    RUN_TEST(test_short_glitch_is_ignored);
    RUN_TEST(test_bouncing_press_counts_as_one_press);
    RUN_TEST(test_bouncing_release_counts_as_one_release);
    RUN_TEST(test_press_count_over_several_presses);
    RUN_TEST(test_threshold_one_reacts_immediately);
    RUN_TEST(test_threshold_zero_counts_as_one);
    RUN_TEST(test_large_threshold);
    RUN_TEST(test_init_resets_everything);
    RUN_TEST(test_null_is_handled);
    RUN_TEST(test_two_buttons_are_independent);
    return UNITY_END();
}
