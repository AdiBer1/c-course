#include <stddef.h>
#include <stdint.h>

#include "course_test.h"
#include "packet.h"

static pkt_parser_t parser;

void setUp(void)
{
    pkt_init(&parser);
}

void tearDown(void) {}

/* Feed bytes; return the result of the LAST byte, and count COMPLETEs. */
static pkt_result_t feed_all(const uint8_t *bytes, size_t n, unsigned *completes)
{
    pkt_result_t r = PKT_NONE;
    for (size_t i = 0; i < n; i++) {
        r = pkt_feed(&parser, bytes[i]);
        if (completes != NULL && r == PKT_COMPLETE) {
            (*completes)++;
        }
    }
    return r;
}

/* ---------------------------------------------------------------- receiving */

static void test_valid_packet_completes(void)
{
    /* LEN 3, payload 01 02 03, checksum 03^01^02^03 = 0x03 */
    const uint8_t frame[] = {0x7E, 0x03, 0x01, 0x02, 0x03, 0x03};
    const uint8_t expected[] = {0x01, 0x02, 0x03};
    uint8_t len = 0;
    TEST_ASSERT_EQUAL_INT(PKT_COMPLETE, feed_all(frame, sizeof frame, NULL));
    const uint8_t *pl = pkt_payload(&parser, &len);
    TEST_ASSERT_NOT_NULL(pl);
    TEST_ASSERT_EQUAL_UINT8(3, len);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, pl, 3);
}

static void test_no_result_before_last_byte(void)
{
    const uint8_t frame[] = {0x7E, 0x02, 0xAA, 0xBB, 0x02 ^ 0xAA ^ 0xBB};
    for (size_t i = 0; i + 1 < sizeof frame; i++) {
        TEST_ASSERT_EQUAL_INT(PKT_NONE, pkt_feed(&parser, frame[i]));
    }
    TEST_ASSERT_EQUAL_INT(PKT_COMPLETE, pkt_feed(&parser, frame[sizeof frame - 1]));
}

static void test_garbage_before_start_is_ignored(void)
{
    const uint8_t stream[] = {0x00, 0xFF, 0x13, 0x7E, 0x01, 0x55, 0x01 ^ 0x55};
    uint8_t len = 0;
    TEST_ASSERT_EQUAL_INT(PKT_COMPLETE, feed_all(stream, sizeof stream, NULL));
    const uint8_t *pl = pkt_payload(&parser, &len);
    TEST_ASSERT_NOT_NULL(pl);
    TEST_ASSERT_EQUAL_HEX8(0x55, pl[0]);
}

static void test_empty_payload(void)
{
    const uint8_t frame[] = {0x7E, 0x00, 0x00};
    uint8_t len = 99;
    TEST_ASSERT_EQUAL_INT(PKT_COMPLETE, feed_all(frame, sizeof frame, NULL));
    TEST_ASSERT_NOT_NULL(pkt_payload(&parser, &len));
    TEST_ASSERT_EQUAL_UINT8(0, len);
}

static void test_start_byte_inside_payload_is_data(void)
{
    const uint8_t frame[] = {0x7E, 0x02, 0x7E, 0x7E, 0x02};
    uint8_t len = 0;
    TEST_ASSERT_EQUAL_INT(PKT_COMPLETE, feed_all(frame, sizeof frame, NULL));
    const uint8_t *pl = pkt_payload(&parser, &len);
    TEST_ASSERT_NOT_NULL(pl);
    TEST_ASSERT_EQUAL_UINT8(2, len);
    TEST_ASSERT_EQUAL_HEX8(0x7E, pl[0]);
    TEST_ASSERT_EQUAL_HEX8(0x7E, pl[1]);
}

static void test_back_to_back_packets(void)
{
    const uint8_t stream[] = {
        0x7E, 0x01, 0x10, 0x11,
        0x7E, 0x01, 0x20, 0x21,
        0x7E, 0x02, 0x30, 0x31, 0x02 ^ 0x30 ^ 0x31,
    };
    unsigned completes = 0;
    uint8_t len = 0;
    feed_all(stream, sizeof stream, &completes);
    TEST_ASSERT_EQUAL_UINT(3, completes);
    const uint8_t *pl = pkt_payload(&parser, &len);
    TEST_ASSERT_NOT_NULL(pl);
    TEST_ASSERT_EQUAL_UINT8(2, len);
    TEST_ASSERT_EQUAL_HEX8(0x30, pl[0]);
    TEST_ASSERT_EQUAL_HEX8(0x31, pl[1]);
}

static void test_maximum_length_payload(void)
{
    uint8_t frame[PKT_MAX_PAYLOAD + 3];
    uint8_t sum = PKT_MAX_PAYLOAD;
    frame[0] = 0x7E;
    frame[1] = PKT_MAX_PAYLOAD;
    for (uint8_t i = 0; i < PKT_MAX_PAYLOAD; i++) {
        frame[2 + i] = (uint8_t)(0xA0 + i);
        sum ^= frame[2 + i];
    }
    frame[PKT_MAX_PAYLOAD + 2] = sum;
    uint8_t len = 0;
    TEST_ASSERT_EQUAL_INT(PKT_COMPLETE, feed_all(frame, sizeof frame, NULL));
    const uint8_t *pl = pkt_payload(&parser, &len);
    TEST_ASSERT_NOT_NULL(pl);
    TEST_ASSERT_EQUAL_UINT8(PKT_MAX_PAYLOAD, len);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(frame + 2, pl, PKT_MAX_PAYLOAD);
}

/* ---------------------------------------------------------------- errors */

static void test_length_too_big_is_an_error(void)
{
    const uint8_t frame[] = {0x7E, PKT_MAX_PAYLOAD + 1};
    TEST_ASSERT_EQUAL_INT(PKT_ERR_LENGTH, feed_all(frame, sizeof frame, NULL));
    TEST_ASSERT_EQUAL_UINT32(1, pkt_error_count(&parser));
}

static void test_bad_checksum_is_an_error(void)
{
    const uint8_t frame[] = {0x7E, 0x02, 0x01, 0x02, 0xFF};
    TEST_ASSERT_EQUAL_INT(PKT_ERR_CHECKSUM, feed_all(frame, sizeof frame, NULL));
    TEST_ASSERT_EQUAL_UINT32(1, pkt_error_count(&parser));
}

static void test_parser_recovers_after_errors(void)
{
    const uint8_t stream[] = {
        0x7E, 0xFF,                         /* bad length */
        0x7E, 0x01, 0x05, 0x00,             /* bad checksum */
        0x7E, 0x01, 0x09, 0x01 ^ 0x09,      /* good */
    };
    unsigned completes = 0;
    uint8_t len = 0;
    feed_all(stream, sizeof stream, &completes);
    TEST_ASSERT_EQUAL_UINT(1, completes);
    TEST_ASSERT_EQUAL_UINT32(2, pkt_error_count(&parser));
    const uint8_t *pl = pkt_payload(&parser, &len);
    TEST_ASSERT_NOT_NULL(pl);
    TEST_ASSERT_EQUAL_HEX8(0x09, pl[0]);
}

static void test_failed_frame_keeps_previous_payload(void)
{
    const uint8_t good[] = {0x7E, 0x01, 0x42, 0x01 ^ 0x42};
    const uint8_t bad[] = {0x7E, 0x01, 0x99, 0x00};
    uint8_t len = 0;
    feed_all(good, sizeof good, NULL);
    feed_all(bad, sizeof bad, NULL);
    const uint8_t *pl = pkt_payload(&parser, &len);
    TEST_ASSERT_NOT_NULL(pl);
    TEST_ASSERT_EQUAL_UINT8(1, len);
    TEST_ASSERT_EQUAL_HEX8(0x42, pl[0]);
}

static void test_no_payload_before_first_packet(void)
{
    uint8_t len = 99;
    TEST_ASSERT_NOT_NULL(pkt_payload(&parser, &len));
    TEST_ASSERT_EQUAL_UINT8(0, len);
}

static void test_null_handling(void)
{
    uint8_t len = 0;
    pkt_init(NULL);
    TEST_ASSERT_EQUAL_INT(PKT_NONE, pkt_feed(NULL, 0x7E));
    TEST_ASSERT_NULL(pkt_payload(NULL, &len));
    TEST_ASSERT_NULL(pkt_payload(&parser, NULL));
    TEST_ASSERT_EQUAL_UINT32(0, pkt_error_count(NULL));
}

/* ---------------------------------------------------------------- building */

static void test_build_frame(void)
{
    const uint8_t payload[] = {0x01, 0x02, 0x03};
    const uint8_t expected[] = {0x7E, 0x03, 0x01, 0x02, 0x03, 0x03};
    uint8_t out[16];
    TEST_ASSERT_EQUAL_size_t(6, pkt_build(out, sizeof out, payload, 3));
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, out, 6);
}

static void test_build_rejects_bad_arguments(void)
{
    const uint8_t payload[PKT_MAX_PAYLOAD + 1] = {0};
    uint8_t out[64];
    TEST_ASSERT_EQUAL_size_t(0, pkt_build(out, 5, payload, 3)); /* needs 6 */
    TEST_ASSERT_EQUAL_size_t(0, pkt_build(NULL, 64, payload, 3));
    TEST_ASSERT_EQUAL_size_t(0, pkt_build(out, 64, NULL, 3));
    TEST_ASSERT_EQUAL_size_t(0, pkt_build(out, 64, payload, PKT_MAX_PAYLOAD + 1));
    TEST_ASSERT_EQUAL_size_t(3, pkt_build(out, 3, NULL, 0));
}

static void test_build_then_parse_round_trip(void)
{
    uint8_t payload[20];
    uint8_t frame[32];
    uint8_t len = 0;
    for (uint8_t i = 0; i < sizeof payload; i++) {
        payload[i] = (uint8_t)(i * 37u);
    }
    size_t n = pkt_build(frame, sizeof frame, payload, sizeof payload);
    TEST_ASSERT_EQUAL_size_t(sizeof payload + 3, n);
    TEST_ASSERT_EQUAL_INT(PKT_COMPLETE, feed_all(frame, n, NULL));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(payload, pkt_payload(&parser, &len), sizeof payload);
    TEST_ASSERT_EQUAL_UINT8(sizeof payload, len);
}

int main(int argc, char **argv)
{
    course_parse_args(argc, argv);
    UNITY_BEGIN();
    RUN_TEST(test_valid_packet_completes);
    RUN_TEST(test_no_result_before_last_byte);
    RUN_TEST(test_garbage_before_start_is_ignored);
    RUN_TEST(test_empty_payload);
    RUN_TEST(test_start_byte_inside_payload_is_data);
    RUN_TEST(test_back_to_back_packets);
    RUN_TEST(test_maximum_length_payload);
    RUN_TEST(test_length_too_big_is_an_error);
    RUN_TEST(test_bad_checksum_is_an_error);
    RUN_TEST(test_parser_recovers_after_errors);
    RUN_TEST(test_failed_frame_keeps_previous_payload);
    RUN_TEST(test_no_payload_before_first_packet);
    RUN_TEST(test_null_handling);
    RUN_TEST(test_build_frame);
    RUN_TEST(test_build_rejects_bad_arguments);
    RUN_TEST(test_build_then_parse_round_trip);
    return UNITY_END();
}
