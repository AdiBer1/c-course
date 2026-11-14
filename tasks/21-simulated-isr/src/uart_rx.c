
#include "uart_rx.h"

#include "sim_uart.h"

/* Shared state */
static volatile uint8_t buffer[UART_RX_BUF_SIZE];
static size_t head;
static size_t tail;
static volatile size_t count;
static volatile size_t lines_ready;
static volatile uint32_t dropped;

void uart_rx_init(void)
{
    head = 0u;
    tail = 0u;
    count = 0u;
    lines_ready = 0u;
    dropped = 0u;

    hal_uart_attach_isr(uart_rx_isr);
}

void uart_rx_isr(void)
{
    uint8_t byte = hal_uart_read_dr();

    if (count >= UART_RX_BUF_SIZE) {
        dropped++;
        return;
    }

    buffer[head] = byte;
    head = (head + 1u) % UART_RX_BUF_SIZE;
    count++;

    if (byte == (uint8_t)'\n') {
        lines_ready++;
    }
}

bool uart_rx_line_ready(void)
{
    bool ready;

    hal_irq_disable();
    ready = (lines_ready > 0u);
    hal_irq_enable();

    return ready;
}

bool uart_rx_read_line(char *dst, size_t dst_size)
{
    size_t copied = 0u;
    bool last_byte_was_cr = false;
    bool last_byte_was_copied = false;

    if (dst == NULL || dst_size == 0u) {
        return false;
    }

    hal_irq_disable();

    if (lines_ready == 0u) {
        hal_irq_enable();
        return false;
    }

    while (count > 0u) {
        uint8_t byte = buffer[tail];

        tail = (tail + 1u) % UART_RX_BUF_SIZE;
        count--;

        if (byte == (uint8_t)'\n') {
            if (last_byte_was_cr && last_byte_was_copied) {
                copied--;
            }
            break;
        }

        last_byte_was_cr = (byte == (uint8_t)'\r');
        last_byte_was_copied = false;

        if (copied < dst_size - 1u) {
            dst[copied] = (char)byte;
            copied++;
            last_byte_was_copied = true;
        }
    }

    dst[copied] = '\0';
    lines_ready--;

    hal_irq_enable();

    return true;
}

size_t uart_rx_available(void)
{
    size_t available;

    hal_irq_disable();
    available = count;
    hal_irq_enable();

    return available;
}

uint32_t uart_rx_dropped(void)
{
    uint32_t total;

    hal_irq_disable();
    total = dropped;
    hal_irq_enable();

    return total;
}
