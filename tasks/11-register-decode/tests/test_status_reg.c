#include <stdint.h>

#include "course_test.h"
#include "status_reg.h"

void setUp(void) {}
void tearDown(void) {}

/* Build a register value from fields, for readable tests. */
static uint32_t reg_of(uint32_t flags, uint32_t mode, uint32_t fifo, uint32_t temp12,
                       uint32_t version)
{
    return (flags & 0xFu) | (mode << 4) | (fifo << 8) | ((temp12 & 0xFFFu) << 16) | (version << 28);
}

/* ---------------------------------------------------------------- decode */

static void test_decode_all_zero(void)
{
    status_t s = status_decode(0u);
    TEST_ASSERT_FALSE(s.ready);
    TEST_ASSERT_FALSE(s.error);
    TEST_ASSERT_FALSE(s.overrun);
    TEST_ASSERT_EQUAL_INT(DEV_MODE_IDLE, s.mode);
    TEST_ASSERT_EQUAL_UINT8(0, s.fifo_level);
    TEST_ASSERT_EQUAL_INT16(0, s.temp_sixteenths);
    TEST_ASSERT_EQUAL_UINT8(0, s.version);
}

static void test_decode_flags_individually(void)
{
    TEST_ASSERT_TRUE(status_decode(0x1u).ready);
    TEST_ASSERT_FALSE(status_decode(0x1u).error);
    TEST_ASSERT_TRUE(status_decode(0x2u).error);
    TEST_ASSERT_FALSE(status_decode(0x2u).ready);
    TEST_ASSERT_TRUE(status_decode(0x4u).overrun);
    TEST_ASSERT_FALSE(status_decode(0x4u).error);
}

static void test_decode_ignores_reserved_bit(void)
{
    status_t s = status_decode(0x8u);
    TEST_ASSERT_FALSE(s.ready);
    TEST_ASSERT_FALSE(s.error);
    TEST_ASSERT_FALSE(s.overrun);
    TEST_ASSERT_EQUAL_INT(DEV_MODE_IDLE, s.mode);
}

static void test_decode_valid_modes(void)
{
    TEST_ASSERT_EQUAL_INT(DEV_MODE_MEASURE, status_decode(reg_of(0, 1, 0, 0, 0)).mode);
    TEST_ASSERT_EQUAL_INT(DEV_MODE_CALIBRATE, status_decode(reg_of(0, 2, 0, 0, 0)).mode);
    TEST_ASSERT_EQUAL_INT(DEV_MODE_SLEEP, status_decode(reg_of(0, 3, 0, 0, 0)).mode);
}

static void test_decode_invalid_modes(void)
{
    for (uint32_t m = 4; m <= 15; m++) {
        TEST_ASSERT_EQUAL_INT(DEV_MODE_INVALID, status_decode(reg_of(0, m, 0, 0, 0)).mode);
    }
}

static void test_decode_fifo_level_and_version(void)
{
    status_t s = status_decode(reg_of(0, 0, 0xC8, 0, 0xA));
    TEST_ASSERT_EQUAL_UINT8(200, s.fifo_level);
    TEST_ASSERT_EQUAL_UINT8(10, s.version);
    s = status_decode(reg_of(0, 0, 0xFF, 0, 0xF));
    TEST_ASSERT_EQUAL_UINT8(255, s.fifo_level);
    TEST_ASSERT_EQUAL_UINT8(15, s.version);
}

static void test_decode_positive_temperature(void)
{
    TEST_ASSERT_EQUAL_INT16(400, status_decode(reg_of(0, 0, 0, 0x190, 0)).temp_sixteenths);
    TEST_ASSERT_EQUAL_INT16(2047, status_decode(reg_of(0, 0, 0, 0x7FF, 0)).temp_sixteenths);
}

static void test_decode_negative_temperature_is_sign_extended(void)
{
    TEST_ASSERT_EQUAL_INT16(-1, status_decode(reg_of(0, 0, 0, 0xFFF, 0)).temp_sixteenths);
    TEST_ASSERT_EQUAL_INT16(-2048, status_decode(reg_of(0, 0, 0, 0x800, 0)).temp_sixteenths);
    TEST_ASSERT_EQUAL_INT16(-160, status_decode(reg_of(0, 0, 0, 0xF60, 0)).temp_sixteenths);
}

static void test_decode_fields_do_not_leak_into_each_other(void)
{
    status_t s = status_decode(0xFFFFFFFFu);
    TEST_ASSERT_TRUE(s.ready);
    TEST_ASSERT_TRUE(s.error);
    TEST_ASSERT_TRUE(s.overrun);
    TEST_ASSERT_EQUAL_INT(DEV_MODE_INVALID, s.mode);
    TEST_ASSERT_EQUAL_UINT8(255, s.fifo_level);
    TEST_ASSERT_EQUAL_INT16(-1, s.temp_sixteenths);
    TEST_ASSERT_EQUAL_UINT8(15, s.version);
}

/* ---------------------------------------------------------------- encode */

static void test_encode_example(void)
{
    status_t s = {
        .ready = true,
        .error = false,
        .overrun = true,
        .mode = DEV_MODE_MEASURE,
        .fifo_level = 0x42,
        .temp_sixteenths = -160,
        .version = 3,
    };
    TEST_ASSERT_EQUAL_HEX32(reg_of(0x5, 1, 0x42, 0xF60, 3), status_encode(&s));
}

static void test_encode_invalid_mode_and_null(void)
{
    status_t s = {.mode = DEV_MODE_INVALID};
    TEST_ASSERT_EQUAL_HEX32(0x000000F0u, status_encode(&s));
    s.mode = (dev_mode_t)9;
    TEST_ASSERT_EQUAL_HEX32(0x000000F0u, status_encode(&s));
    TEST_ASSERT_EQUAL_HEX32(0u, status_encode(NULL));
}

static void test_encode_clamps_temperature(void)
{
    status_t s = {.temp_sixteenths = 5000};
    TEST_ASSERT_EQUAL_INT16(2047, status_decode(status_encode(&s)).temp_sixteenths);
    s.temp_sixteenths = -5000;
    TEST_ASSERT_EQUAL_INT16(-2048, status_decode(status_encode(&s)).temp_sixteenths);
}

static void test_encode_uses_only_low_four_bits_of_version(void)
{
    status_t s = {.version = 0x1F};
    TEST_ASSERT_EQUAL_HEX32(0xF0000000u, status_encode(&s));
}

static void test_decode_encode_round_trip(void)
{
    const uint32_t samples[] = {0x00000000u, 0x12345671u, 0x7FF0FF37u,
                                0xA8001302u, 0xFFFFFFF7u, 0x0190C811u};
    for (unsigned i = 0; i < sizeof samples / sizeof samples[0]; i++) {
        uint32_t reg = samples[i] & ~0x8u; /* reserved bit encodes as 0 */
        status_t s = status_decode(reg);
        if (s.mode == DEV_MODE_INVALID) {
            reg |= 0xF0u;
        }
        TEST_ASSERT_EQUAL_HEX32(reg, status_encode(&s));
    }
}

/* ---------------------------------------------------------------- read / celsius */

static void test_read_through_volatile_pointer(void)
{
    volatile uint32_t fake_register = reg_of(0x1, 2, 7, 0x190, 1);
    status_t s = status_read(&fake_register);
    TEST_ASSERT_TRUE(s.ready);
    TEST_ASSERT_EQUAL_INT(DEV_MODE_CALIBRATE, s.mode);
    TEST_ASSERT_EQUAL_UINT8(7, s.fifo_level);
    TEST_ASSERT_EQUAL_INT16(400, s.temp_sixteenths);
}

static void test_read_null_acts_like_zero(void)
{
    status_t s = status_read(NULL);
    TEST_ASSERT_FALSE(s.ready);
    TEST_ASSERT_EQUAL_INT(DEV_MODE_IDLE, s.mode);
}

static void test_temp_celsius_rounds_to_nearest(void)
{
    status_t s = {.temp_sixteenths = 400}; /* 25.0 */
    TEST_ASSERT_EQUAL_INT16(25, status_temp_celsius(&s));
    s.temp_sixteenths = 408; /* 25.5 -> 26 */
    TEST_ASSERT_EQUAL_INT16(26, status_temp_celsius(&s));
    s.temp_sixteenths = 407; /* 25.44 -> 25 */
    TEST_ASSERT_EQUAL_INT16(25, status_temp_celsius(&s));
    s.temp_sixteenths = -8; /* -0.5 -> -1 */
    TEST_ASSERT_EQUAL_INT16(-1, status_temp_celsius(&s));
    s.temp_sixteenths = -7; /* -0.44 -> 0 */
    TEST_ASSERT_EQUAL_INT16(0, status_temp_celsius(&s));
    s.temp_sixteenths = -2048; /* -128.0 */
    TEST_ASSERT_EQUAL_INT16(-128, status_temp_celsius(&s));
    TEST_ASSERT_EQUAL_INT16(0, status_temp_celsius(NULL));
}

int main(int argc, char **argv)
{
    course_parse_args(argc, argv);
    UNITY_BEGIN();
    RUN_TEST(test_decode_all_zero);
    RUN_TEST(test_decode_flags_individually);
    RUN_TEST(test_decode_ignores_reserved_bit);
    RUN_TEST(test_decode_valid_modes);
    RUN_TEST(test_decode_invalid_modes);
    RUN_TEST(test_decode_fifo_level_and_version);
    RUN_TEST(test_decode_positive_temperature);
    RUN_TEST(test_decode_negative_temperature_is_sign_extended);
    RUN_TEST(test_decode_fields_do_not_leak_into_each_other);
    RUN_TEST(test_encode_example);
    RUN_TEST(test_encode_invalid_mode_and_null);
    RUN_TEST(test_encode_clamps_temperature);
    RUN_TEST(test_encode_uses_only_low_four_bits_of_version);
    RUN_TEST(test_decode_encode_round_trip);
    RUN_TEST(test_read_through_volatile_pointer);
    RUN_TEST(test_read_null_acts_like_zero);
    RUN_TEST(test_temp_celsius_rounds_to_nearest);
    return UNITY_END();
}
