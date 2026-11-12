#include "uart_rx.h"

#include "sim_uart.h"

/* Static storage and shared variables */
static volatile uint8_t rx_buffer[UART_RX_BUF_SIZE];
static volatile size_t rx_head = 0u;
static volatile size_t rx_tail = 0u;
static volatile size_t rx_count = 0u;
static volatile uint32_t lines_ready = 0u;
static volatile uint32_t dropped_bytes = 0u;

void uart_rx_init(void)
{
    hal_irq_disable();
    rx_head = 0u;
    rx_tail = 0u;
    rx_count = 0u;
    lines_ready = 0u;
    dropped_bytes = 0u;
    for (size_t i = 0u; i < UART_RX_BUF_SIZE; i++) {
        rx_buffer[i] = 0u;
    }
    hal_uart_attach_isr(uart_rx_isr);
    hal_irq_enable();
}

void uart_rx_isr(void)
{
    uint8_t byte = hal_uart_read_dr();

    if (rx_count >= UART_RX_BUF_SIZE) {
        dropped_bytes++;
        return;
    }

    rx_buffer[rx_head] = byte;
    rx_head = (rx_head + 1u) & (UART_RX_BUF_SIZE - 1u);
    rx_count++;

    if (byte == (uint8_t)'\n') {
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

    // Find the next newline in the buffer
    size_t temp_tail = rx_tail;
    size_t temp_count = rx_count;
    size_t line_len = 0u;
    bool found = false;

    for (size_t i = 0u; i < temp_count; i++) {
        uint8_t b = rx_buffer[temp_tail];
        temp_tail = (temp_tail + 1u) & (UART_RX_BUF_SIZE - 1u);
        line_len++;
        if (b == (uint8_t)'\n') {
            found = true;
            break;
        }
    }

    if (!found) {
        hal_irq_enable();
        return false;
    }

    // Extract the line into dst
    size_t dest_idx = 0u;
    size_t processed = 0u;
    bool newline_reached = false;

    while (rx_count > 0u && !newline_reached) {
        uint8_t b = rx_buffer[rx_tail];
        rx_tail = (rx_tail + 1u) & (UART_RX_BUF_SIZE - 1u);
        rx_count--;
        processed++;

        if (b == (uint8_t)'\n') {
            newline_reached = true;
            lines_ready--;
        } else {
            // Skip carriage return '\r' if it appears right before '\n'
            if (b == (uint8_t)'\r' && rx_count > 0u && rx_buffer[rx_tail] == (uint8_t)'\n') {
                continue;
            }
            if (dest_idx < dst_size - 1u) {
                dst[dest_idx] = (char)b;
                dest_idx++;
            }
        }
    }

    dst[dest_idx] = '\0';
    hal_irq_enable();
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
    uint32_t dropped = dropped_bytes;
    hal_irq_enable();
    return dropped;
}