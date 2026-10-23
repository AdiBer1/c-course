#include <stddef.h>
#include <stdint.h>

#include "course_test.h"
#include "crc8.h"

void setUp(void) {}
void tearDown(void) {}

static const uint8_t check_string[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

static void test_update_single_bytes(void)
{
    TEST_ASSERT_EQUAL_HEX8(0x00, crc8_update(0x00, 0x00));
    TEST_ASSERT_EQUAL_HEX8(0x07, crc8_update(0x00, 0x01));
    TEST_ASSERT_EQUAL_HEX8(0xF3, crc8_update(0x00, 0xFF));
    TEST_ASSERT_EQUAL_HEX8(0x89, crc8_update(0x00, 0x80));
}

static void test_update_continues_from_previous_crc(void)
{
    TEST_ASSERT_EQUAL_HEX8(0x1B, crc8_update(0x07, 0x02));
}

static void test_standard_check_value(void)
{
    TEST_ASSERT_EQUAL_HEX8(0xF4, crc8(check_string, sizeof check_string));
}

static void test_empty_and_null_input(void)
{
    TEST_ASSERT_EQUAL_HEX8(CRC8_INIT, crc8(check_string, 0));
    TEST_ASSERT_EQUAL_HEX8(CRC8_INIT, crc8(NULL, 0));
    TEST_ASSERT_EQUAL_HEX8(CRC8_INIT, crc8(NULL, 10));
}

static void test_known_short_messages(void)
{
    const uint8_t a[] = {0xBE, 0xEF};
    const uint8_t b[] = {0x00, 0x00, 0x00, 0x00};
    const uint8_t c[] = {0xFF, 0xFF, 0xFF, 0xFF};
    TEST_ASSERT_EQUAL_HEX8(0x1A, crc8(a, sizeof a));
    TEST_ASSERT_EQUAL_HEX8(0x00, crc8(b, sizeof b));
    TEST_ASSERT_EQUAL_HEX8(0xDE, crc8(c, sizeof c));
}

static void test_streaming_gives_the_same_result(void)
{
    uint8_t crc = CRC8_INIT;
    for (size_t i = 0; i < sizeof check_string; i++) {
        crc = crc8_update(crc, check_string[i]);
    }
    TEST_ASSERT_EQUAL_HEX8(crc8(check_string, sizeof check_string), crc);
}

static void test_detects_every_single_bit_error(void)
{
    uint8_t msg[9];
    for (size_t i = 0; i < sizeof msg; i++) {
        msg[i] = check_string[i];
    }
    const uint8_t good = crc8(msg, sizeof msg);
    for (size_t byte = 0; byte < sizeof msg; byte++) {
        for (unsigned bit = 0; bit < 8; bit++) {
            msg[byte] ^= (uint8_t)(1u << bit);
            TEST_ASSERT_NOT_EQUAL(good, crc8(msg, sizeof msg));
            msg[byte] ^= (uint8_t)(1u << bit);
        }
    }
}

static void test_detects_swapped_bytes(void)
{
    const uint8_t ab[] = {0x12, 0x34};
    const uint8_t ba[] = {0x34, 0x12};
    TEST_ASSERT_NOT_EQUAL(crc8(ab, 2), crc8(ba, 2));
}

static void test_frame_with_correct_crc_is_ok(void)
{
    uint8_t frame[4] = {0x01, 0x02, 0x03, 0x00};
    frame[3] = crc8(frame, 3);
    TEST_ASSERT_TRUE(crc8_frame_ok(frame, sizeof frame));
}

static void test_frame_with_wrong_crc_is_rejected(void)
{
    uint8_t frame[4] = {0x01, 0x02, 0x03, 0x00};
    frame[3] = (uint8_t)(crc8(frame, 3) ^ 0x01u);
    TEST_ASSERT_FALSE(crc8_frame_ok(frame, sizeof frame));
}

static void test_frame_corrupted_in_data_is_rejected(void)
{
    uint8_t frame[4] = {0x01, 0x02, 0x03, 0x00};
    frame[3] = crc8(frame, 3);
    frame[1] = 0x22;
    TEST_ASSERT_FALSE(crc8_frame_ok(frame, sizeof frame));
}

static void test_frame_too_short_or_null_is_rejected(void)
{
    const uint8_t one[1] = {0x00};
    TEST_ASSERT_FALSE(crc8_frame_ok(one, 1));
    TEST_ASSERT_FALSE(crc8_frame_ok(one, 0));
    TEST_ASSERT_FALSE(crc8_frame_ok(NULL, 5));
}

static void test_sensor_style_frame(void)
{
    /* A 16-bit reading 0xBEEF followed by its CRC, like many I2C sensors send. */
    const uint8_t frame[3] = {0xBE, 0xEF, 0x1A};
    TEST_ASSERT_TRUE(crc8_frame_ok(frame, 3));
}

int main(int argc, char **argv)
{
    course_parse_args(argc, argv);
    UNITY_BEGIN();
    RUN_TEST(test_update_single_bytes);
    RUN_TEST(test_update_continues_from_previous_crc);
    RUN_TEST(test_standard_check_value);
    RUN_TEST(test_empty_and_null_input);
    RUN_TEST(test_known_short_messages);
    RUN_TEST(test_streaming_gives_the_same_result);
    RUN_TEST(test_detects_every_single_bit_error);
    RUN_TEST(test_detects_swapped_bytes);
    RUN_TEST(test_frame_with_correct_crc_is_ok);
    RUN_TEST(test_frame_with_wrong_crc_is_rejected);
    RUN_TEST(test_frame_corrupted_in_data_is_rejected);
    RUN_TEST(test_frame_too_short_or_null_is_rejected);
    RUN_TEST(test_sensor_style_frame);
    return UNITY_END();
}
