#include "uart_rx.h"

#include "sim_uart.h"

static volatile uint8_t rx_buffer[UART_RX_BUF_SIZE];
static volatile size_t rx_head;
static volatile size_t rx_tail;
static volatile size_t rx_count;
static volatile uint32_t rx_lines_ready;
static volatile uint32_t rx_dropped;

static size_t advance_index(size_t index)
{
    return (index + 1u) & (UART_RX_BUF_SIZE - 1u);
}

void uart_rx_init(void)
{
    hal_irq_disable();

    rx_head = 0u;
    rx_tail = 0u;
    rx_count = 0u;
    rx_lines_ready = 0u;
    rx_dropped = 0u;

    hal_uart_attach_isr(uart_rx_isr);

    hal_irq_enable();
}

void uart_rx_isr(void)
{
    uint8_t byte = hal_uart_read_dr();

    if (rx_count >= UART_RX_BUF_SIZE) {
        rx_dropped++;
        return;
    }

    rx_buffer[rx_head] = byte;
    rx_head = advance_index(rx_head);
    rx_count++;

    if (byte == '\n') {
        rx_lines_ready++;
    }
}

bool uart_rx_line_ready(void)
{
    return rx_lines_ready > 0u;
}

bool uart_rx_read_line(char *dst, size_t dst_size)
{
    size_t dst_pos = 0u;
    bool result = false;

    if (dst == NULL || dst_size == 0u) {
        return false;
    }

    hal_irq_disable();

    if (rx_lines_ready == 0u) {
        hal_irq_enable();
        return false;
    }

    while (rx_count > 0u) {
        uint8_t byte = rx_buffer[rx_tail];

        rx_tail = advance_index(rx_tail);
        rx_count--;

        if (byte == '\n') {
            if (dst_pos > 0u && dst[dst_pos - 1u] == '\r') {
                dst_pos--;
            }

            dst[dst_pos] = '\0';
            rx_lines_ready--;
            result = true;
            break;
        }

        if (dst_pos + 1u < dst_size) {
            dst[dst_pos] = (char)byte;
            dst_pos++;
        }
    }

    hal_irq_enable();

    return result;
}

size_t uart_rx_available(void)
{
    return rx_count;
}

uint32_t uart_rx_dropped(void)
{
    return rx_dropped;
}