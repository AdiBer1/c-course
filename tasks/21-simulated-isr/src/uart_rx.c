#include "uart_rx.h"
#include "sim_uart.h"

static volatile char ring_buf[UART_RX_BUF_SIZE];
static volatile size_t head;
static volatile size_t tail;
static volatile size_t count;
static volatile size_t lines_ready;
static volatile uint32_t dropped_count;

void uart_rx_init(void)
{
    hal_irq_disable();
    head = 0;
    tail = 0;
    count = 0;
    lines_ready = 0;
    dropped_count = 0;
    hal_irq_enable();

    hal_uart_attach_isr(uart_rx_isr);
}

void uart_rx_isr(void)
{
    char byte = (char)hal_uart_read_dr();

    if (count >= UART_RX_BUF_SIZE) {
        dropped_count++;
        return;
    }

    ring_buf[head] = byte;
    head = (head + 1u) % UART_RX_BUF_SIZE;
    count++;

    if (byte == '\n') {
        lines_ready++;
    }
}

bool uart_rx_line_ready(void)
{
    hal_irq_disable();
    bool ready = (lines_ready > 0);
    hal_irq_enable();
    return ready;
}

bool uart_rx_read_line(char *dst, size_t dst_size)
{
    if (dst == NULL || dst_size == 0) {
        return false;
    }

    hal_irq_disable();
    if (lines_ready == 0) {
        hal_irq_enable();
        return false;
    }
    hal_irq_enable();

    size_t written = 0;
    bool found_newline = false;
    bool prev_was_cr = false;

    while (!found_newline) {
        hal_irq_disable();
        if (count == 0) {
            hal_irq_enable();
            break;
        }
        char ch = ring_buf[tail];
        tail = (tail + 1u) % UART_RX_BUF_SIZE;
        count--;
        hal_irq_enable();

        if (ch == '\n') {
            found_newline = true;
            if (prev_was_cr && written > 0) {
                written--;
            }
        } else {
            if (ch == '\r') {
                prev_was_cr = true;
            } else {
                prev_was_cr = false;
            }

            if (written < dst_size - 1u) {
                dst[written] = ch;
                written++;
            }
        }
    }

    dst[written] = '\0';

    hal_irq_disable();
    if (lines_ready > 0) {
        lines_ready--;
    }
    hal_irq_enable();

    return true;
}

size_t uart_rx_available(void)
{
    hal_irq_disable();
    size_t avail = count;
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