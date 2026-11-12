#include "uart_rx.h"

#include "sim_uart.h"

static volatile uint8_t rx_buf[UART_RX_BUF_SIZE];
static volatile size_t rx_head;
static volatile size_t rx_tail;
static volatile size_t rx_count;
static volatile size_t rx_lines_ready;
static volatile uint32_t rx_dropped;

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

    rx_buf[rx_head] = byte;
    rx_head = (rx_head + 1u) & (UART_RX_BUF_SIZE - 1u);
    rx_count++;

    if (byte == (uint8_t)'\n') {
        rx_lines_ready++;
    }
}

bool uart_rx_line_ready(void)
{
    return rx_lines_ready > 0u;
}

bool uart_rx_read_line(char *dst, size_t dst_size)
{
    if (dst == NULL || dst_size == 0u) {
        return false;
    }

    hal_irq_disable();

    if (rx_lines_ready == 0u) {
        hal_irq_enable();
        return false;
    }

    size_t out_idx = 0u;
    bool prev_was_cr = false;

    while (rx_count > 0u) {
        uint8_t byte = rx_buf[rx_tail];
        rx_tail = (rx_tail + 1u) & (UART_RX_BUF_SIZE - 1u);
        rx_count--;

        if (byte == (uint8_t)'\n') {
            if (prev_was_cr && out_idx > 0u) {
                out_idx--;
            }
            rx_lines_ready--;
            break;
        }

        if (out_idx + 1u < dst_size) {
            dst[out_idx] = (char)byte;
            out_idx++;
            prev_was_cr = (byte == (uint8_t)'\r');
        } else {
            prev_was_cr = false;
        }
    }

    dst[out_idx] = '\0';

    hal_irq_enable();
    return true;
}

size_t uart_rx_available(void)
{
    hal_irq_disable();
    size_t avail = rx_count;
    hal_irq_enable();
    return avail;
}

uint32_t uart_rx_dropped(void)
{
    hal_irq_disable();
    uint32_t dropped = rx_dropped;
    hal_irq_enable();
    return dropped;
}