#include <stdint.h>
#include <string.h>

#include "course_test.h"
#include "byte_order.h"

void setUp(void) {}
void tearDown(void) {}

static const uint8_t bytes[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};

/* ---------------------------------------------------------------- reading */

static void test_read_u16_little_endian(void)
{
    TEST_ASSERT_EQUAL_HEX16(0x3412, rd_u16_le(bytes));
}

static void test_read_u16_big_endian(void)
{
    TEST_ASSERT_EQUAL_HEX16(0x1234, rd_u16_be(bytes));
}

static void test_read_u32_little_endian(void)
{
    TEST_ASSERT_EQUAL_HEX32(0x78563412u, rd_u32_le(bytes));
}

static void test_read_u32_big_endian(void)
{
    TEST_ASSERT_EQUAL_HEX32(0x12345678u, rd_u32_be(bytes));
}

static void test_read_with_high_bit_set(void)
{
    /* 0xFF << 24 must not go through a signed int (undefined behaviour). */
    const uint8_t ff[] = {0xFF, 0xFF, 0xFF, 0xFF};
    TEST_ASSERT_EQUAL_HEX32(0xFFFFFFFFu, rd_u32_be(ff));
    TEST_ASSERT_EQUAL_HEX32(0xFFFFFFFFu, rd_u32_le(ff));
    TEST_ASSERT_EQUAL_HEX16(0xFFFF, rd_u16_be(ff));
    const uint8_t hi[] = {0x9A, 0xBC, 0xDE, 0xF0};
    TEST_ASSERT_EQUAL_HEX32(0x9ABCDEF0u, rd_u32_be(hi));
    TEST_ASSERT_EQUAL_HEX32(0xF0DEBC9Au, rd_u32_le(hi));
}

static void test_read_from_unaligned_address(void)
{
    TEST_ASSERT_EQUAL_HEX32(0x3456789Au, rd_u32_be(bytes + 1));
    TEST_ASSERT_EQUAL_HEX32(0xDEBC9A78u, rd_u32_le(bytes + 3));
    TEST_ASSERT_EQUAL_HEX16(0x5634, rd_u16_le(bytes + 1));
}

static void test_read_signed_big_endian(void)
{
    const uint8_t neg[] = {0xFF, 0x38};
    const uint8_t pos[] = {0x01, 0x2C};
    const uint8_t min[] = {0x80, 0x00};
    TEST_ASSERT_EQUAL_INT16(-200, rd_i16_be(neg));
    TEST_ASSERT_EQUAL_INT16(300, rd_i16_be(pos));
    TEST_ASSERT_EQUAL_INT16(INT16_MIN, rd_i16_be(min));
}

/* ---------------------------------------------------------------- writing */

static void test_write_u16_both_orders(void)
{
    uint8_t buf[2];
    wr_u16_le(buf, 0xBEEF);
    TEST_ASSERT_EQUAL_HEX8(0xEF, buf[0]);
    TEST_ASSERT_EQUAL_HEX8(0xBE, buf[1]);
    wr_u16_be(buf, 0xBEEF);
    TEST_ASSERT_EQUAL_HEX8(0xBE, buf[0]);
    TEST_ASSERT_EQUAL_HEX8(0xEF, buf[1]);
}

static void test_write_u32_little_endian(void)
{
    uint8_t buf[4];
    const uint8_t expected[] = {0x0D, 0xF0, 0xFE, 0xCA};
    wr_u32_le(buf, 0xCAFEF00Du);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, buf, 4);
}

static void test_write_u32_big_endian(void)
{
    uint8_t buf[4];
    const uint8_t expected[] = {0xCA, 0xFE, 0xF0, 0x0D};
    wr_u32_be(buf, 0xCAFEF00Du);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, buf, 4);
}

static void test_write_touches_only_its_bytes(void)
{
    uint8_t buf[8];
    memset(buf, 0xAA, sizeof buf);
    wr_u32_be(buf + 2, 0x01020304u);
    const uint8_t expected[] = {0xAA, 0xAA, 0x01, 0x02, 0x03, 0x04, 0xAA, 0xAA};
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, buf, 8);
    wr_u16_le(buf + 6, 0x5566);
    TEST_ASSERT_EQUAL_HEX8(0x66, buf[6]);
    TEST_ASSERT_EQUAL_HEX8(0x55, buf[7]);
}

static void test_round_trip_many_values(void)
{
    uint8_t buf[4];
    uint32_t v = 1u;
    for (int i = 0; i < 1000; i++) {
        v = v * 1664525u + 1013904223u; /* pseudo-random sequence */
        wr_u32_le(buf, v);
        TEST_ASSERT_EQUAL_HEX32(v, rd_u32_le(buf));
        wr_u32_be(buf, v);
        TEST_ASSERT_EQUAL_HEX32(v, rd_u32_be(buf));
        wr_u16_be(buf, (uint16_t)v);
        TEST_ASSERT_EQUAL_HEX16((uint16_t)v, rd_u16_be(buf));
    }
}

/* ---------------------------------------------------------------- host order */

static void test_host_byte_order_detection(void)
{
    const uint32_t probe = 0x01020304u;
    uint8_t first;
    memcpy(&first, &probe, 1); /* the test may peek at memory; your code may not use memcpy */
    TEST_ASSERT_EQUAL(first == 0x04, host_is_little_endian());
}

int main(int argc, char **argv)
{
    course_parse_args(argc, argv);
    UNITY_BEGIN();
    RUN_TEST(test_read_u16_little_endian);
    RUN_TEST(test_read_u16_big_endian);
    RUN_TEST(test_read_u32_little_endian);
    RUN_TEST(test_read_u32_big_endian);
    RUN_TEST(test_read_with_high_bit_set);
    RUN_TEST(test_read_from_unaligned_address);
    RUN_TEST(test_read_signed_big_endian);
    RUN_TEST(test_write_u16_both_orders);
    RUN_TEST(test_write_u32_little_endian);
    RUN_TEST(test_write_u32_big_endian);
    RUN_TEST(test_write_touches_only_its_bytes);
    RUN_TEST(test_round_trip_many_values);
    RUN_TEST(test_host_byte_order_detection);
    return UNITY_END();
}
