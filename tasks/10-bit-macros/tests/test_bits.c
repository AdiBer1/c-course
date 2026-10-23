#include <stdint.h>

#include "bits.h"
#include "course_test.h"

void setUp(void) {}
void tearDown(void) {}

/* ---------------------------------------------------------------- single bits */

static void test_bit_values(void)
{
    TEST_ASSERT_EQUAL_HEX32(0x00000001u, BIT(0));
    TEST_ASSERT_EQUAL_HEX32(0x00000020u, BIT(5));
    TEST_ASSERT_EQUAL_HEX32(0x80000000u, BIT(31));
}

static void test_bit_is_a_32_bit_unsigned_value(void)
{
    TEST_ASSERT_EQUAL_size_t(sizeof(uint32_t), sizeof(BIT(3)));
    TEST_ASSERT_TRUE(BIT(31) > 0);
}

static void test_bit_set(void)
{
    uint32_t reg = 0x00000000u;
    reg = BIT_SET(reg, 3);
    TEST_ASSERT_EQUAL_HEX32(0x00000008u, reg);
    reg = BIT_SET(reg, 31);
    TEST_ASSERT_EQUAL_HEX32(0x80000008u, reg);
    reg = BIT_SET(reg, 3); /* already set: no change */
    TEST_ASSERT_EQUAL_HEX32(0x80000008u, reg);
}

static void test_bit_clear(void)
{
    uint32_t reg = 0xFFFFFFFFu;
    reg = BIT_CLEAR(reg, 0);
    TEST_ASSERT_EQUAL_HEX32(0xFFFFFFFEu, reg);
    reg = BIT_CLEAR(reg, 31);
    TEST_ASSERT_EQUAL_HEX32(0x7FFFFFFEu, reg);
    reg = BIT_CLEAR(reg, 0); /* already clear: no change */
    TEST_ASSERT_EQUAL_HEX32(0x7FFFFFFEu, reg);
}

static void test_bit_toggle(void)
{
    uint32_t reg = 0x0000000Fu;
    reg = BIT_TOGGLE(reg, 0);
    TEST_ASSERT_EQUAL_HEX32(0x0000000Eu, reg);
    reg = BIT_TOGGLE(reg, 8);
    TEST_ASSERT_EQUAL_HEX32(0x0000010Eu, reg);
    reg = BIT_TOGGLE(BIT_TOGGLE(reg, 20), 20);
    TEST_ASSERT_EQUAL_HEX32(0x0000010Eu, reg);
}

static void test_bit_read(void)
{
    const uint32_t reg = 0x80000005u;
    TEST_ASSERT_EQUAL_UINT32(1u, BIT_READ(reg, 0));
    TEST_ASSERT_EQUAL_UINT32(0u, BIT_READ(reg, 1));
    TEST_ASSERT_EQUAL_UINT32(1u, BIT_READ(reg, 2));
    TEST_ASSERT_EQUAL_UINT32(1u, BIT_READ(reg, 31));
}

static void test_macros_are_fully_parenthesised(void)
{
    uint32_t a = 0x1u;
    uint32_t b = 0x2u;
    TEST_ASSERT_EQUAL_HEX32(0x8u, BIT(1 + 2));
    TEST_ASSERT_EQUAL_HEX32(0xFFFFFFF7u, ~BIT(3));
    TEST_ASSERT_EQUAL_HEX32(0x7u, BIT_SET(a | b, 2));
    TEST_ASSERT_EQUAL_HEX32(0x2u, BIT_CLEAR(a | b, 1 - 1));
    TEST_ASSERT_EQUAL_UINT32(2u, 2u * BIT_READ(a | b, 1));
    TEST_ASSERT_EQUAL_HEX32(0x0u, BIT_TOGGLE(a, 0) & 0x1u);
}

/* ---------------------------------------------------------------- fields */

static void test_field_mask(void)
{
    TEST_ASSERT_EQUAL_HEX32(0x00000070u, FIELD_MASK(4, 3));
    TEST_ASSERT_EQUAL_HEX32(0x00000001u, FIELD_MASK(0, 1));
    TEST_ASSERT_EQUAL_HEX32(0x80000000u, FIELD_MASK(31, 1));
    TEST_ASSERT_EQUAL_HEX32(0xFFFF0000u, FIELD_MASK(16, 16));
}

static void test_field_mask_full_width(void)
{
    TEST_ASSERT_EQUAL_HEX32(0xFFFFFFFFu, FIELD_MASK(0, 32));
}

static void test_field_get(void)
{
    TEST_ASSERT_EQUAL_HEX32(0x56u, FIELD_GET(0x12345678u, 8, 8));
    TEST_ASSERT_EQUAL_HEX32(0x1u, FIELD_GET(0x12345678u, 28, 4));
    TEST_ASSERT_EQUAL_HEX32(0x0u, FIELD_GET(0x12345678u, 0, 3));
    TEST_ASSERT_EQUAL_HEX32(0x12345678u, FIELD_GET(0x12345678u, 0, 32));
    TEST_ASSERT_EQUAL_HEX32(0x7u, FIELD_GET(0x12345678u, 2 + 2, 1 + 2));
}

static void test_field_set(void)
{
    TEST_ASSERT_EQUAL_HEX32(0xFFFFFF0Fu, FIELD_SET(0xFFFFFFFFu, 4, 4, 0x0u));
    TEST_ASSERT_EQUAL_HEX32(0x12AB5678u, FIELD_SET(0x12345678u, 16, 8, 0xABu));
    TEST_ASSERT_EQUAL_HEX32(0xA0000000u, FIELD_SET(0x00000000u, 28, 4, 0xAu));
    TEST_ASSERT_EQUAL_HEX32(0xCAFEF00Du, FIELD_SET(0x12345678u, 0, 32, 0xCAFEF00Du));
}

static void test_field_set_ignores_extra_bits_of_value(void)
{
    /* 0x1F does not fit in a 3-bit field: only 0x7 is used. */
    TEST_ASSERT_EQUAL_HEX32(0x00000070u, FIELD_SET(0x00000000u, 4, 3, 0x1Fu));
    TEST_ASSERT_EQUAL_HEX32(0xFFFFFFFFu, FIELD_SET(0xFFFFFFFFu, 4, 3, 0xFFu));
}

static void test_field_set_then_get_round_trip(void)
{
    uint32_t reg = 0xDEADBEEFu;
    reg = FIELD_SET(reg, 9, 5, 0x13u);
    TEST_ASSERT_EQUAL_HEX32(0x13u, FIELD_GET(reg, 9, 5));
    /* all other bits unchanged */
    TEST_ASSERT_EQUAL_HEX32(0xDEADBEEFu & ~FIELD_MASK(9, 5), reg & ~FIELD_MASK(9, 5));
}

/* ---------------------------------------------------------------- functions */

static void test_count_ones(void)
{
    TEST_ASSERT_EQUAL_UINT(0, bits_count_ones(0u));
    TEST_ASSERT_EQUAL_UINT(1, bits_count_ones(0x80000000u));
    TEST_ASSERT_EQUAL_UINT(8, bits_count_ones(0x0000F0F0u));
    TEST_ASSERT_EQUAL_UINT(32, bits_count_ones(0xFFFFFFFFu));
}

static void test_is_power_of_two(void)
{
    TEST_ASSERT_FALSE(bits_is_power_of_two(0u));
    TEST_ASSERT_TRUE(bits_is_power_of_two(1u));
    TEST_ASSERT_TRUE(bits_is_power_of_two(64u));
    TEST_ASSERT_TRUE(bits_is_power_of_two(0x80000000u));
    TEST_ASSERT_FALSE(bits_is_power_of_two(6u));
    TEST_ASSERT_FALSE(bits_is_power_of_two(0xFFFFFFFFu));
}

static void test_swap_nibbles(void)
{
    TEST_ASSERT_EQUAL_HEX8(0x5Au, bits_swap_nibbles(0xA5u));
    TEST_ASSERT_EQUAL_HEX8(0x10u, bits_swap_nibbles(0x01u));
    TEST_ASSERT_EQUAL_HEX8(0x0Fu, bits_swap_nibbles(0xF0u));
    TEST_ASSERT_EQUAL_HEX8(0xFFu, bits_swap_nibbles(0xFFu));
}

int main(int argc, char **argv)
{
    course_parse_args(argc, argv);
    UNITY_BEGIN();
    RUN_TEST(test_bit_values);
    RUN_TEST(test_bit_is_a_32_bit_unsigned_value);
    RUN_TEST(test_bit_set);
    RUN_TEST(test_bit_clear);
    RUN_TEST(test_bit_toggle);
    RUN_TEST(test_bit_read);
    RUN_TEST(test_macros_are_fully_parenthesised);
    RUN_TEST(test_field_mask);
    RUN_TEST(test_field_mask_full_width);
    RUN_TEST(test_field_get);
    RUN_TEST(test_field_set);
    RUN_TEST(test_field_set_ignores_extra_bits_of_value);
    RUN_TEST(test_field_set_then_get_round_trip);
    RUN_TEST(test_count_ones);
    RUN_TEST(test_is_power_of_two);
    RUN_TEST(test_swap_nibbles);
    return UNITY_END();
}
