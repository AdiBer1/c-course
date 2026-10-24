#include <stddef.h>
#include <stdint.h>

#include "course_test.h"
#include "mock_clock.h"
#include "mock_gpio.h"
#include "traffic_light.h"

static mock_gpio_t gpio_mock;
static hal_gpio_t gpio;
static mock_clock_t clock_mock;
static hal_clock_t clk;
static traffic_light_t tl;

static void start_at(uint32_t start_ms)
{
    mock_gpio_init(&gpio_mock, &gpio);
    mock_clock_init(&clock_mock, &clk, start_ms);
    TEST_ASSERT_TRUE(tl_init(&tl, &gpio, &clk));
}

void setUp(void)
{
    start_at(1000u);
}

void tearDown(void) {}

/* Advance time by ms and run one update. */
static void step(uint32_t ms)
{
    mock_clock_advance(&clock_mock, ms);
    tl_update(&tl);
}

static void assert_lamps(gpio_level_t red, gpio_level_t yellow, gpio_level_t green)
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(red, mock_gpio_output(&gpio_mock, TL_PIN_RED), "red lamp");
    TEST_ASSERT_EQUAL_INT_MESSAGE(yellow, mock_gpio_output(&gpio_mock, TL_PIN_YELLOW),
                                  "yellow lamp");
    TEST_ASSERT_EQUAL_INT_MESSAGE(green, mock_gpio_output(&gpio_mock, TL_PIN_GREEN), "green lamp");
}

static void press_button(void)
{
    mock_gpio_set_input(&gpio_mock, TL_PIN_BUTTON, GPIO_HIGH);
}

static void release_button(void)
{
    mock_gpio_set_input(&gpio_mock, TL_PIN_BUTTON, GPIO_LOW);
}

/* ---------------------------------------------------------------- start */

static void test_starts_red_with_red_lamp(void)
{
    TEST_ASSERT_EQUAL_INT(TL_RED, tl_phase(&tl));
    assert_lamps(GPIO_HIGH, GPIO_LOW, GPIO_LOW);
    TEST_ASSERT_FALSE(tl_pedestrian_waiting(&tl));
}

static void test_init_rejects_bad_arguments(void)
{
    traffic_light_t other;
    hal_clock_t no_clock = {NULL, NULL};
    hal_gpio_t no_read = {gpio.write, NULL, gpio.ctx};
    TEST_ASSERT_FALSE(tl_init(NULL, &gpio, &clk));
    TEST_ASSERT_FALSE(tl_init(&other, NULL, &clk));
    TEST_ASSERT_FALSE(tl_init(&other, &gpio, NULL));
    TEST_ASSERT_FALSE(tl_init(&other, &gpio, &no_clock));
    TEST_ASSERT_FALSE(tl_init(&other, &no_read, &clk));
}

/* ---------------------------------------------------------------- timing */

static void test_stays_red_until_time_is_up(void)
{
    step(TL_RED_MS - 1);
    TEST_ASSERT_EQUAL_INT(TL_RED, tl_phase(&tl));
    step(1);
    TEST_ASSERT_EQUAL_INT(TL_RED_YELLOW, tl_phase(&tl));
    assert_lamps(GPIO_HIGH, GPIO_HIGH, GPIO_LOW);
}

static void test_full_cycle_with_lamps(void)
{
    step(TL_RED_MS);
    TEST_ASSERT_EQUAL_INT(TL_RED_YELLOW, tl_phase(&tl));
    step(TL_RED_YELLOW_MS);
    TEST_ASSERT_EQUAL_INT(TL_GREEN, tl_phase(&tl));
    assert_lamps(GPIO_LOW, GPIO_LOW, GPIO_HIGH);
    step(TL_GREEN_MS);
    TEST_ASSERT_EQUAL_INT(TL_YELLOW, tl_phase(&tl));
    assert_lamps(GPIO_LOW, GPIO_HIGH, GPIO_LOW);
    step(TL_YELLOW_MS);
    TEST_ASSERT_EQUAL_INT(TL_RED, tl_phase(&tl));
    assert_lamps(GPIO_HIGH, GPIO_LOW, GPIO_LOW);
}

static void test_many_small_steps(void)
{
    /* the main loop calls update every 10 ms */
    for (uint32_t t = 0; t < TL_RED_MS + TL_RED_YELLOW_MS - 10; t += 10) {
        step(10);
    }
    TEST_ASSERT_EQUAL_INT(TL_RED_YELLOW, tl_phase(&tl));
    step(10);
    TEST_ASSERT_EQUAL_INT(TL_GREEN, tl_phase(&tl));
}

static void test_update_without_time_passing_returns_immediately(void)
{
    /* With a mock clock time never moves by itself: a waiting loop would hang here. */
    for (int i = 0; i < 1000; i++) {
        tl_update(&tl);
    }
    TEST_ASSERT_EQUAL_INT(TL_RED, tl_phase(&tl));
}

static void test_at_most_one_phase_change_per_update(void)
{
    step(60000); /* a very late update */
    TEST_ASSERT_EQUAL_INT(TL_RED_YELLOW, tl_phase(&tl));
    tl_update(&tl); /* no time passed since the change */
    TEST_ASSERT_EQUAL_INT(TL_RED_YELLOW, tl_phase(&tl));
}

static void test_new_phase_starts_at_the_time_of_the_change(void)
{
    step(TL_RED_MS + 300); /* change happens 300 ms late */
    TEST_ASSERT_EQUAL_INT(TL_RED_YELLOW, tl_phase(&tl));
    step(TL_RED_YELLOW_MS - 1);
    TEST_ASSERT_EQUAL_INT(TL_RED_YELLOW, tl_phase(&tl));
    step(1);
    TEST_ASSERT_EQUAL_INT(TL_GREEN, tl_phase(&tl));
}

static void test_works_across_clock_wrap_around(void)
{
    start_at(0xFFFFFFFFu - 2000u); /* the counter wraps 2 s into RED */
    step(100);                     /* "now >= start + duration" wrongly says RED is over here */
    TEST_ASSERT_EQUAL_INT(TL_RED, tl_phase(&tl));
    step(2900);
    TEST_ASSERT_EQUAL_INT(TL_RED, tl_phase(&tl));
    step(TL_RED_MS - 3000);
    TEST_ASSERT_EQUAL_INT(TL_RED_YELLOW, tl_phase(&tl));
    step(TL_RED_YELLOW_MS);
    TEST_ASSERT_EQUAL_INT(TL_GREEN, tl_phase(&tl));
}

/* ---------------------------------------------------------------- pedestrian button */

static void reach_green(void)
{
    step(TL_RED_MS);
    step(TL_RED_YELLOW_MS);
    TEST_ASSERT_EQUAL_INT(TL_GREEN, tl_phase(&tl));
}

static void test_button_during_green_shortens_it(void)
{
    reach_green();
    step(500);
    press_button();
    step(1);
    release_button();
    TEST_ASSERT_TRUE(tl_pedestrian_waiting(&tl));
    TEST_ASSERT_EQUAL_INT(TL_GREEN, tl_phase(&tl)); /* minimum green not reached */
    step(TL_GREEN_MIN_MS - 501);
    TEST_ASSERT_EQUAL_INT(TL_YELLOW, tl_phase(&tl));
    TEST_ASSERT_FALSE(tl_pedestrian_waiting(&tl));
}

static void test_button_late_in_green_ends_it_at_once(void)
{
    reach_green();
    step(2000);
    press_button();
    step(1);
    TEST_ASSERT_EQUAL_INT(TL_YELLOW, tl_phase(&tl));
}

static void test_button_during_red_is_remembered(void)
{
    press_button();
    step(100);
    release_button();
    TEST_ASSERT_TRUE(tl_pedestrian_waiting(&tl));
    step(TL_RED_MS);
    step(TL_RED_YELLOW_MS);
    TEST_ASSERT_EQUAL_INT(TL_GREEN, tl_phase(&tl));
    step(TL_GREEN_MIN_MS);
    TEST_ASSERT_EQUAL_INT(TL_YELLOW, tl_phase(&tl));
}

static void test_without_button_green_lasts_full_time(void)
{
    reach_green();
    step(TL_GREEN_MS - 1);
    TEST_ASSERT_EQUAL_INT(TL_GREEN, tl_phase(&tl));
    TEST_ASSERT_FALSE(tl_pedestrian_waiting(&tl));
}

static void test_red_is_never_shortened(void)
{
    press_button();
    step(TL_RED_MS - 1);
    TEST_ASSERT_EQUAL_INT(TL_RED, tl_phase(&tl));
}

static void test_null_is_safe(void)
{
    tl_update(NULL);
    TEST_ASSERT_EQUAL_INT(TL_RED, tl_phase(NULL));
    TEST_ASSERT_FALSE(tl_pedestrian_waiting(NULL));
}

int main(int argc, char **argv)
{
    course_parse_args(argc, argv);
    UNITY_BEGIN();
    RUN_TEST(test_starts_red_with_red_lamp);
    RUN_TEST(test_init_rejects_bad_arguments);
    RUN_TEST(test_stays_red_until_time_is_up);
    RUN_TEST(test_full_cycle_with_lamps);
    RUN_TEST(test_many_small_steps);
    RUN_TEST(test_update_without_time_passing_returns_immediately);
    RUN_TEST(test_at_most_one_phase_change_per_update);
    RUN_TEST(test_new_phase_starts_at_the_time_of_the_change);
    RUN_TEST(test_works_across_clock_wrap_around);
    RUN_TEST(test_button_during_green_shortens_it);
    RUN_TEST(test_button_late_in_green_ends_it_at_once);
    RUN_TEST(test_button_during_red_is_remembered);
    RUN_TEST(test_without_button_green_lasts_full_time);
    RUN_TEST(test_red_is_never_shortened);
    RUN_TEST(test_null_is_safe);
    return UNITY_END();
}
