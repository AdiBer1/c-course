#include "uart_rx.h"
#include "sim_uart.h"

static char rx_buf[UART_RX_BUF_SIZE];
static volatile size_t head = 0;
static volatile size_t tail = 0;
static volatile size_t buffered_count = 0;
static volatile uint32_t lines_ready_count = 0;
static volatile uint32_t dropped_count = 0;

void uart_rx_init(void)
{
    hal_irq_disable();

    head = 0;
    tail = 0;
    buffered_count = 0;
    lines_ready_count = 0;
    dropped_count = 0;

    for (size_t i = 0; i < UART_RX_BUF_SIZE; i++) {
        rx_buf[i] = '\0';
    }

    hal_uart_attach_isr(uart_rx_isr);

    hal_irq_enable();
}

void uart_rx_isr(void)
{
    uint8_t byte = hal_uart_read_dr();

    if (buffered_count >= UART_RX_BUF_SIZE) {
        dropped_count++;
    } else {
        rx_buf[head] = (char)byte;
        head = (head + 1u) & (UART_RX_BUF_SIZE - 1u);
        buffered_count++;

        if (byte == (uint8_t)'\n') {
            lines_ready_count++;
        }
    }
}

bool uart_rx_line_ready(void)
{
    hal_irq_disable();
    bool ready = (lines_ready_count > 0u);
    hal_irq_enable();
    return ready;
}

bool uart_rx_read_line(char *dst, size_t dst_size)
{
    if (dst == NULL || dst_size == 0u) {
        return false;
    }

    hal_irq_disable();
    if (lines_ready_count == 0u) {
        hal_irq_enable();
        return false;
    }
    hal_irq_enable();

    size_t copied = 0;
    bool line_complete = false;

    while (!line_complete) {
        hal_irq_disable();

        if (buffered_count == 0u) {
            hal_irq_enable();
            break;
        }

        char c = rx_buf[tail];
        tail = (tail + 1u) & (UART_RX_BUF_SIZE - 1u);
        buffered_count--;

        if (c == '\n') {
            if (lines_ready_count > 0u) {
                lines_ready_count--;
            }
            line_complete = true;
            hal_irq_enable();
            break;
        }

        hal_irq_enable();

        if (c == '\r') {
            continue;
        }

        if (copied < (dst_size - 1u)) {
            dst[copied] = c;
            copied++;
        }
    }

    if (!line_complete) {
        return false;
    }

    dst[copied] = '\0';
    return true;
}

size_t uart_rx_available(void)
{
    hal_irq_disable();
    size_t avail = buffered_count;
    hal_irq_enable();
    return avail;
}

uint32_t uart_rx_dropped(void)
{
    hal_irq_disable();
    uint32_t dropped = dropped_count;
    hal_irq_enable();
    return dropped;
}