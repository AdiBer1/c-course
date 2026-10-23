#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "course_test.h"
#include "sim_uart.h"
#include "uart_rx.h"

void setUp(void)
{
    sim_uart_reset();
    uart_rx_init();
}

void tearDown(void) {}

/* The outside world sends a string, byte by byte, each byte firing the RX interrupt. */
static void send(const char *s)
{
    while (*s != '\0') {
        sim_uart_receive((uint8_t)*s);
        s++;
    }
}

/* ---------------------------------------------------------------- the ISR */

static void test_init_attaches_the_isr(void)
{
    sim_uart_receive('a');
    TEST_ASSERT_EQUAL_UINT32(1, sim_isr_runs());
    TEST_ASSERT_EQUAL_size_t(1, uart_rx_available());
}

static void test_isr_always_reads_the_data_register(void)
{
    send("hello");
    TEST_ASSERT_EQUAL_UINT32(0, sim_isr_unread());
}

static void test_bytes_are_buffered(void)
{
    send("abc");
    TEST_ASSERT_EQUAL_size_t(3, uart_rx_available());
    TEST_ASSERT_FALSE(uart_rx_line_ready());
}

static void test_newline_makes_a_line_ready(void)
{
    send("LED=1");
    TEST_ASSERT_FALSE(uart_rx_line_ready());
    send("\n");
    TEST_ASSERT_TRUE(uart_rx_line_ready());
}

/* ---------------------------------------------------------------- reading lines */

static void test_read_line_returns_text_without_newline(void)
{
    char line[32];
    send("LED=1\n");
    TEST_ASSERT_TRUE(uart_rx_read_line(line, sizeof line));
    TEST_ASSERT_EQUAL_STRING("LED=1", line);
    TEST_ASSERT_EQUAL_size_t(0, uart_rx_available());
    TEST_ASSERT_FALSE(uart_rx_line_ready());
}

static void test_read_line_strips_carriage_return(void)
{
    char line[32];
    send("STATUS\r\n");
    TEST_ASSERT_TRUE(uart_rx_read_line(line, sizeof line));
    TEST_ASSERT_EQUAL_STRING("STATUS", line);
}

static void test_no_complete_line_returns_false_and_keeps_bytes(void)
{
    char line[8] = "xyz";
    send("par");
    TEST_ASSERT_FALSE(uart_rx_read_line(line, sizeof line));
    TEST_ASSERT_EQUAL_STRING("xyz", line);
    TEST_ASSERT_EQUAL_size_t(3, uart_rx_available());
    send("t\n");
    TEST_ASSERT_TRUE(uart_rx_read_line(line, sizeof line));
    TEST_ASSERT_EQUAL_STRING("part", line);
}

static void test_two_lines_are_read_in_order(void)
{
    char line[16];
    send("A=1\nB=2\n");
    TEST_ASSERT_TRUE(uart_rx_read_line(line, sizeof line));
    TEST_ASSERT_EQUAL_STRING("A=1", line);
    TEST_ASSERT_TRUE(uart_rx_line_ready());
    TEST_ASSERT_TRUE(uart_rx_read_line(line, sizeof line));
    TEST_ASSERT_EQUAL_STRING("B=2", line);
    TEST_ASSERT_FALSE(uart_rx_line_ready());
}

static void test_empty_line(void)
{
    char line[4] = "zz";
    send("\n");
    TEST_ASSERT_TRUE(uart_rx_read_line(line, sizeof line));
    TEST_ASSERT_EQUAL_STRING("", line);
}

static void test_long_line_is_truncated_and_rest_discarded(void)
{
    char line[4];
    send("abcdefgh\nok\n");
    TEST_ASSERT_TRUE(uart_rx_read_line(line, sizeof line));
    TEST_ASSERT_EQUAL_STRING("abc", line);
    TEST_ASSERT_TRUE(uart_rx_read_line(line, sizeof line));
    TEST_ASSERT_EQUAL_STRING("ok", line);
}

static void test_read_line_bad_arguments(void)
{
    char line[4];
    send("x\n");
    TEST_ASSERT_FALSE(uart_rx_read_line(NULL, 4));
    TEST_ASSERT_FALSE(uart_rx_read_line(line, 0));
    TEST_ASSERT_TRUE(uart_rx_line_ready());
}

/* ---------------------------------------------------------------- full buffer */

static void test_full_buffer_drops_and_counts(void)
{
    for (int i = 0; i < 20; i++) {
        sim_uart_receive('x');
    }
    TEST_ASSERT_EQUAL_size_t(UART_RX_BUF_SIZE, uart_rx_available());
    TEST_ASSERT_EQUAL_UINT32(20 - UART_RX_BUF_SIZE, uart_rx_dropped());
    TEST_ASSERT_EQUAL_UINT32(0, sim_isr_unread());
}

static void test_buffer_space_is_reused_after_reading(void)
{
    char line[UART_RX_BUF_SIZE];
    for (int round = 0; round < 10; round++) {
        send("0123456789\n"); /* 11 bytes: wraps around the 16-byte buffer */
        TEST_ASSERT_TRUE(uart_rx_read_line(line, sizeof line));
        TEST_ASSERT_EQUAL_STRING("0123456789", line);
    }
    TEST_ASSERT_EQUAL_UINT32(0, uart_rx_dropped());
}

static void test_init_resets_everything(void)
{
    send("abc\n");
    for (int i = 0; i < 20; i++) {
        sim_uart_receive('x');
    }
    uart_rx_init();
    TEST_ASSERT_EQUAL_size_t(0, uart_rx_available());
    TEST_ASSERT_EQUAL_UINT32(0, uart_rx_dropped());
    TEST_ASSERT_FALSE(uart_rx_line_ready());
}

/* ---------------------------------------------------------------- critical sections */

static void test_read_line_uses_a_critical_section(void)
{
    char line[8];
    send("go\n");
    uint32_t before = sim_irq_disable_calls();
    TEST_ASSERT_TRUE(uart_rx_read_line(line, sizeof line));
    TEST_ASSERT_TRUE_MESSAGE(sim_irq_disable_calls() > before,
                             "uart_rx_read_line must disable interrupts while it changes shared state");
    TEST_ASSERT_TRUE_MESSAGE(sim_irq_enabled(), "interrupts must be enabled again when it returns");
}

static void test_interrupts_enabled_after_every_call(void)
{
    char line[8];
    (void)uart_rx_read_line(line, sizeof line); /* nothing there */
    TEST_ASSERT_TRUE(sim_irq_enabled());
    (void)uart_rx_available();
    TEST_ASSERT_TRUE(sim_irq_enabled());
    (void)uart_rx_line_ready();
    TEST_ASSERT_TRUE(sim_irq_enabled());
}

static void test_byte_arriving_during_critical_section_is_not_lost(void)
{
    char line[8];
    send("ab");
    hal_irq_disable(); /* pretend the main loop is inside a critical section */
    sim_uart_receive('\n');
    TEST_ASSERT_EQUAL_UINT32(2, sim_isr_runs()); /* the ISR has not run yet */
    hal_irq_enable(); /* now the pending interrupt is taken */
    TEST_ASSERT_TRUE(uart_rx_read_line(line, sizeof line));
    TEST_ASSERT_EQUAL_STRING("ab", line);
}

static void test_isr_interleaved_with_main_loop(void)
{
    /* Bytes keep arriving between main-loop calls, like on a real device. */
    char line[16];
    const char *stream = "T=21\nT=22\nT=23\n";
    unsigned lines = 0;
    for (const char *p = stream; *p != '\0'; p++) {
        sim_uart_receive((uint8_t)*p);
        if (uart_rx_read_line(line, sizeof line)) {
            lines++;
            TEST_ASSERT_EQUAL_CHAR('T', line[0]);
            TEST_ASSERT_EQUAL_size_t(4, strlen(line));
        }
    }
    TEST_ASSERT_EQUAL_UINT(3, lines);
    TEST_ASSERT_EQUAL_size_t(0, uart_rx_available());
}

int main(int argc, char **argv)
{
    course_parse_args(argc, argv);
    UNITY_BEGIN();
    RUN_TEST(test_init_attaches_the_isr);
    RUN_TEST(test_isr_always_reads_the_data_register);
    RUN_TEST(test_bytes_are_buffered);
    RUN_TEST(test_newline_makes_a_line_ready);
    RUN_TEST(test_read_line_returns_text_without_newline);
    RUN_TEST(test_read_line_strips_carriage_return);
    RUN_TEST(test_no_complete_line_returns_false_and_keeps_bytes);
    RUN_TEST(test_two_lines_are_read_in_order);
    RUN_TEST(test_empty_line);
    RUN_TEST(test_long_line_is_truncated_and_rest_discarded);
    RUN_TEST(test_read_line_bad_arguments);
    RUN_TEST(test_full_buffer_drops_and_counts);
    RUN_TEST(test_buffer_space_is_reused_after_reading);
    RUN_TEST(test_init_resets_everything);
    RUN_TEST(test_read_line_uses_a_critical_section);
    RUN_TEST(test_interrupts_enabled_after_every_call);
    RUN_TEST(test_byte_arriving_during_critical_section_is_not_lost);
    RUN_TEST(test_isr_interleaved_with_main_loop);
    return UNITY_END();
}
