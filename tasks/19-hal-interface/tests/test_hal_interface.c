#include <stddef.h>

#include "course_test.h"
#include "fake_gpio.h"
#include "led.h"
#include "mock_gpio.h"

static mock_gpio_t mock;
static hal_gpio_t mock_hal;

void setUp(void)
{
    mock_gpio_init(&mock, &mock_hal);
}

void tearDown(void) {}

/* ================================================================ LED driver (teacher's mock) */

static void test_led_init_turns_led_off_active_high(void)
{
    led_t led;
    mock.outputs[3] = GPIO_HIGH;
    TEST_ASSERT_TRUE(led_init(&led, &mock_hal, 3, false));
    TEST_ASSERT_EQUAL_INT(GPIO_LOW, mock_gpio_output(&mock, 3));
    TEST_ASSERT_FALSE(led_is_on(&led));
    TEST_ASSERT_EQUAL_UINT32(1, mock.write_count);
}

static void test_led_init_turns_led_off_active_low(void)
{
    led_t led;
    TEST_ASSERT_TRUE(led_init(&led, &mock_hal, 5, true));
    TEST_ASSERT_EQUAL_INT(GPIO_HIGH, mock_gpio_output(&mock, 5));
    TEST_ASSERT_FALSE(led_is_on(&led));
}

static void test_led_set_active_high(void)
{
    led_t led;
    led_init(&led, &mock_hal, 2, false);
    led_set(&led, true);
    TEST_ASSERT_EQUAL_INT(GPIO_HIGH, mock_gpio_output(&mock, 2));
    TEST_ASSERT_TRUE(led_is_on(&led));
    led_set(&led, false);
    TEST_ASSERT_EQUAL_INT(GPIO_LOW, mock_gpio_output(&mock, 2));
    TEST_ASSERT_FALSE(led_is_on(&led));
    TEST_ASSERT_EQUAL_UINT32(3, mock.write_count);
}

static void test_led_set_active_low(void)
{
    led_t led;
    led_init(&led, &mock_hal, 7, true);
    led_set(&led, true);
    TEST_ASSERT_EQUAL_INT(GPIO_LOW, mock_gpio_output(&mock, 7));
    TEST_ASSERT_TRUE(led_is_on(&led));
}

static void test_led_toggle(void)
{
    led_t led;
    led_init(&led, &mock_hal, 1, false);
    led_toggle(&led);
    TEST_ASSERT_TRUE(led_is_on(&led));
    TEST_ASSERT_EQUAL_INT(GPIO_HIGH, mock_gpio_output(&mock, 1));
    led_toggle(&led);
    TEST_ASSERT_FALSE(led_is_on(&led));
    TEST_ASSERT_EQUAL_INT(GPIO_LOW, mock_gpio_output(&mock, 1));
}

static void test_led_writes_only_its_own_pin(void)
{
    led_t led;
    led_init(&led, &mock_hal, 9, false);
    led_set(&led, true);
    for (uint8_t pin = 0; pin < MOCK_GPIO_PINS; pin++) {
        if (pin != 9) {
            TEST_ASSERT_EQUAL_INT(GPIO_LOW, mock_gpio_output(&mock, pin));
        }
    }
    TEST_ASSERT_EQUAL_UINT8(9, mock.last_pin);
}

static void test_two_leds_on_the_same_port(void)
{
    led_t red;
    led_t green;
    led_init(&red, &mock_hal, 0, false);
    led_init(&green, &mock_hal, 1, true);
    led_set(&red, true);
    TEST_ASSERT_EQUAL_INT(GPIO_HIGH, mock_gpio_output(&mock, 0));
    TEST_ASSERT_EQUAL_INT(GPIO_HIGH, mock_gpio_output(&mock, 1)); /* green still off (active low) */
    TEST_ASSERT_FALSE(led_is_on(&green));
}

static void test_led_init_rejects_bad_arguments(void)
{
    led_t led;
    hal_gpio_t broken = {NULL, NULL, NULL};
    TEST_ASSERT_FALSE(led_init(NULL, &mock_hal, 0, false));
    TEST_ASSERT_FALSE(led_init(&led, NULL, 0, false));
    TEST_ASSERT_FALSE(led_init(&led, &broken, 0, false));
    TEST_ASSERT_EQUAL_UINT32(0, mock.write_count);
}

static void test_led_null_is_safe(void)
{
    led_set(NULL, true);
    led_toggle(NULL);
    TEST_ASSERT_FALSE(led_is_on(NULL));
}

/* ================================================================ button driver */

static void test_button_active_high(void)
{
    button_t b;
    TEST_ASSERT_TRUE(button_init(&b, &mock_hal, 4, false));
    TEST_ASSERT_FALSE(button_is_pressed(&b));
    mock_gpio_set_input(&mock, 4, GPIO_HIGH);
    TEST_ASSERT_TRUE(button_is_pressed(&b));
}

static void test_button_active_low(void)
{
    button_t b;
    mock_gpio_set_input(&mock, 4, GPIO_HIGH);
    TEST_ASSERT_TRUE(button_init(&b, &mock_hal, 4, true));
    TEST_ASSERT_FALSE(button_is_pressed(&b));
    mock_gpio_set_input(&mock, 4, GPIO_LOW);
    TEST_ASSERT_TRUE(button_is_pressed(&b));
}

static void test_button_reads_the_pin_every_call(void)
{
    button_t b;
    button_init(&b, &mock_hal, 6, false);
    (void)button_is_pressed(&b);
    (void)button_is_pressed(&b);
    (void)button_is_pressed(&b);
    TEST_ASSERT_EQUAL_UINT32(3, mock.read_count);
}

static void test_button_rejects_bad_arguments(void)
{
    button_t b;
    hal_gpio_t no_read = {mock_hal.write, NULL, &mock};
    TEST_ASSERT_FALSE(button_init(NULL, &mock_hal, 0, false));
    TEST_ASSERT_FALSE(button_init(&b, NULL, 0, false));
    TEST_ASSERT_FALSE(button_init(&b, &no_read, 0, false));
    TEST_ASSERT_FALSE(button_is_pressed(NULL));
}

/* ================================================================ your fake GPIO */

static void test_fake_init_fills_the_interface(void)
{
    fake_gpio_t fake;
    hal_gpio_t hal = {NULL, NULL, NULL};
    fake_gpio_init(&fake, &hal);
    TEST_ASSERT_NOT_NULL(hal.write);
    TEST_ASSERT_NOT_NULL(hal.read);
    TEST_ASSERT_EQUAL_PTR(&fake, hal.ctx);
    TEST_ASSERT_EQUAL_size_t(0, fake.log_count);
    TEST_ASSERT_EQUAL_UINT32(0, fake.total_writes);
}

static void test_fake_logs_writes_in_order(void)
{
    fake_gpio_t fake;
    hal_gpio_t hal = {NULL, NULL, NULL};
    fake_gpio_init(&fake, &hal);
    TEST_ASSERT_NOT_NULL(hal.write);
    hal.write(hal.ctx, 3, GPIO_HIGH);
    hal.write(hal.ctx, 1, GPIO_LOW);
    TEST_ASSERT_EQUAL_size_t(2, fake.log_count);
    TEST_ASSERT_EQUAL_UINT8(3, fake.log[0].pin);
    TEST_ASSERT_EQUAL_INT(GPIO_HIGH, fake.log[0].level);
    TEST_ASSERT_EQUAL_UINT8(1, fake.log[1].pin);
    TEST_ASSERT_EQUAL_INT(GPIO_LOW, fake.log[1].level);
}

static void test_fake_log_does_not_overflow(void)
{
    fake_gpio_t fake;
    hal_gpio_t hal = {NULL, NULL, NULL};
    fake_gpio_init(&fake, &hal);
    TEST_ASSERT_NOT_NULL(hal.write);
    for (uint8_t i = 0; i < 20; i++) {
        hal.write(hal.ctx, i, GPIO_HIGH);
    }
    TEST_ASSERT_EQUAL_size_t(FAKE_GPIO_LOG_SIZE, fake.log_count);
    TEST_ASSERT_EQUAL_UINT32(20, fake.total_writes);
    TEST_ASSERT_EQUAL_UINT8(FAKE_GPIO_LOG_SIZE - 1, fake.log[FAKE_GPIO_LOG_SIZE - 1].pin);
}

static void test_fake_read_returns_input_levels(void)
{
    fake_gpio_t fake;
    hal_gpio_t hal = {NULL, NULL, NULL};
    fake_gpio_init(&fake, &hal);
    TEST_ASSERT_NOT_NULL(hal.read);
    TEST_ASSERT_EQUAL_INT(GPIO_LOW, hal.read(hal.ctx, 2));
    fake.input_level[2] = GPIO_HIGH;
    TEST_ASSERT_EQUAL_INT(GPIO_HIGH, hal.read(hal.ctx, 2));
    TEST_ASSERT_EQUAL_INT(GPIO_LOW, hal.read(hal.ctx, 200)); /* out of range */
    TEST_ASSERT_EQUAL_UINT32(3, fake.reads);
}

static void test_two_fakes_are_independent(void)
{
    fake_gpio_t a;
    fake_gpio_t b;
    hal_gpio_t hal_a = {NULL, NULL, NULL};
    hal_gpio_t hal_b = {NULL, NULL, NULL};
    fake_gpio_init(&a, &hal_a);
    fake_gpio_init(&b, &hal_b);
    TEST_ASSERT_NOT_NULL(hal_a.write);
    hal_a.write(hal_a.ctx, 1, GPIO_HIGH);
    TEST_ASSERT_EQUAL_size_t(1, a.log_count);
    TEST_ASSERT_EQUAL_size_t(0, b.log_count);
}

static void test_fake_init_null_is_safe(void)
{
    fake_gpio_t fake;
    hal_gpio_t hal = {NULL, NULL, NULL};
    fake_gpio_init(NULL, &hal);
    fake_gpio_init(&fake, NULL);
    TEST_ASSERT_NULL(hal.write);
}

/* ================================================================ both sides together */

static void test_led_driver_on_your_fake(void)
{
    fake_gpio_t fake;
    hal_gpio_t hal = {NULL, NULL, NULL};
    led_t led;
    fake_gpio_init(&fake, &hal);
    TEST_ASSERT_TRUE(led_init(&led, &hal, 4, true));
    led_set(&led, true);
    led_toggle(&led);
    /* init: off (HIGH, active low), set on: LOW, toggle -> off: HIGH */
    TEST_ASSERT_EQUAL_size_t(3, fake.log_count);
    TEST_ASSERT_EQUAL_INT(GPIO_HIGH, fake.log[0].level);
    TEST_ASSERT_EQUAL_INT(GPIO_LOW, fake.log[1].level);
    TEST_ASSERT_EQUAL_INT(GPIO_HIGH, fake.log[2].level);
    TEST_ASSERT_EQUAL_UINT8(4, fake.log[2].pin);
}

static void test_button_driver_on_your_fake(void)
{
    fake_gpio_t fake;
    hal_gpio_t hal = {NULL, NULL, NULL};
    button_t b;
    fake_gpio_init(&fake, &hal);
    TEST_ASSERT_TRUE(button_init(&b, &hal, 0, false));
    fake.input_level[0] = GPIO_HIGH;
    TEST_ASSERT_TRUE(button_is_pressed(&b));
}

int main(int argc, char **argv)
{
    course_parse_args(argc, argv);
    UNITY_BEGIN();
    RUN_TEST(test_led_init_turns_led_off_active_high);
    RUN_TEST(test_led_init_turns_led_off_active_low);
    RUN_TEST(test_led_set_active_high);
    RUN_TEST(test_led_set_active_low);
    RUN_TEST(test_led_toggle);
    RUN_TEST(test_led_writes_only_its_own_pin);
    RUN_TEST(test_two_leds_on_the_same_port);
    RUN_TEST(test_led_init_rejects_bad_arguments);
    RUN_TEST(test_led_null_is_safe);
    RUN_TEST(test_button_active_high);
    RUN_TEST(test_button_active_low);
    RUN_TEST(test_button_reads_the_pin_every_call);
    RUN_TEST(test_button_rejects_bad_arguments);
    RUN_TEST(test_fake_init_fills_the_interface);
    RUN_TEST(test_fake_logs_writes_in_order);
    RUN_TEST(test_fake_log_does_not_overflow);
    RUN_TEST(test_fake_read_returns_input_levels);
    RUN_TEST(test_two_fakes_are_independent);
    RUN_TEST(test_fake_init_null_is_safe);
    RUN_TEST(test_led_driver_on_your_fake);
    RUN_TEST(test_button_driver_on_your_fake);
    return UNITY_END();
}
