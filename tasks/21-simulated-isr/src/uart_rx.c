#include "uart_rx.h"

#include "sim_uart.h"

/*
 * TODO:
 *   - static storage: the buffer, indices/count, the "lines ready" counter,
 *     the dropped counter. Think about which ones need `volatile`.
 *   - implement every function; protect the main-loop side with
 *     hal_irq_disable() / hal_irq_enable()
 */


#include <stddef.h>

static volatile char rx_buffer[UART_RX_BUF_SIZE];
static volatile size_t rx_head = 0;
static volatile size_t rx_tail = 0;
static volatile size_t rx_count = 0;
static volatile uint32_t dropped_count = 0;
static volatile size_t lines_ready = 0;

void uart_rx_init(void)
{
    hal_irq_disable();
    rx_head = 0;
    rx_tail = 0;
    rx_count = 0;
    dropped_count = 0;
    lines_ready = 0;
    hal_uart_attach_isr(uart_rx_isr);
    hal_irq_enable();
}

void uart_rx_isr(void)
{
    char c = (char)hal_uart_read_dr();

    if (rx_count >= UART_RX_BUF_SIZE) {
        dropped_count++;
        return;
    }

    rx_buffer[rx_head] = c;
    rx_head = (rx_head + 1u) % UART_RX_BUF_SIZE;
    rx_count++;

    if (c == '\n') {
        lines_ready++;
    }
}

bool uart_rx_line_ready(void)
{
    hal_irq_disable();
    bool ready = (lines_ready > 0u);
    hal_irq_enable();
    return ready;
}

bool uart_rx_read_line(char *dst, size_t dst_size)
{
    if (dst == NULL || dst_size == 0u) {
        return false;
    }

    hal_irq_disable();
    if (lines_ready == 0u) {
        hal_irq_enable();
        return false;
    }
    hal_irq_enable();

    bool found_newline = false;
    char temp_buf[UART_RX_BUF_SIZE];
    size_t temp_len = 0u;

    hal_irq_disable();

    while (rx_count > 0u) {
        char c = rx_buffer[rx_tail];
        rx_tail = (rx_tail + 1u) % UART_RX_BUF_SIZE;
        rx_count--;
        temp_buf[temp_len++] = c;

        if (c == '\n') {
            lines_ready--;
            found_newline = true;
            break;
        }
    }

    hal_irq_enable();

    if (!found_newline) {
        hal_irq_disable();
        size_t i = temp_len;
        while (i > 0u) {
            i--;
            rx_tail = (rx_tail == 0u) ? (UART_RX_BUF_SIZE - 1u) : (rx_tail - 1u);
            rx_buffer[rx_tail] = temp_buf[i];
            rx_count++;
        }
        hal_irq_enable();
        return false;
    }

    if (temp_len > 0u && temp_buf[temp_len - 1u] == '\n') {
        temp_len--;
    }
    if (temp_len > 0u && temp_buf[temp_len - 1u] == '\r') {
        temp_len--;
    }

    size_t max_copy = (dst_size - 1u < temp_len) ? (dst_size - 1u) : temp_len;
    for (size_t i = 0u; i < max_copy; i++) {
        dst[i] = temp_buf[i];
    }
    dst[max_copy] = '\0';

    return true;
}

size_t uart_rx_available(void)
{
    hal_irq_disable();
    size_t count = rx_count;
    hal_irq_enable();
    return count;
}

uint32_t uart_rx_dropped(void)
{
    hal_irq_disable();
    uint32_t dropped = dropped_count;
    hal_irq_enable();
    return dropped;
}