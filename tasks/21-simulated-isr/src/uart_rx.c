#include "uart_rx.h"

#include "sim_uart.h"

#define BUF_MASK (UART_RX_BUF_SIZE - 1u)

/* Shared between the ISR and the main loop: all volatile. */
static volatile uint8_t buf[UART_RX_BUF_SIZE];
static volatile size_t head;  /* ISR writes here */
static volatile size_t tail;  /* main loop reads here */
static volatile size_t count; /* bytes buffered */
static volatile size_t lines; /* complete lines buffered */
static volatile uint32_t dropped;

void uart_rx_init(void)
{
    size_t i;

    hal_irq_disable();
    for (i = 0; i < UART_RX_BUF_SIZE; i++) {
        buf[i] = 0u;
    }
    head = 0u;
    tail = 0u;
    count = 0u;
    lines = 0u;
    dropped = 0u;
    hal_irq_enable();

    hal_uart_attach_isr(uart_rx_isr);
}

void uart_rx_isr(void)
{
    uint8_t byte = (uint8_t)hal_uart_read_dr(); /* always read it */

    if (count >= UART_RX_BUF_SIZE) {
        dropped++;
        return;
    }
    buf[head] = byte;
    head = (head + 1u) & BUF_MASK;
    count++;
    if (byte == (uint8_t)'\n') {
        lines++;
    }
}

bool uart_rx_line_ready(void)
{
    return lines > 0u;
}

bool uart_rx_read_line(char *dst, size_t dst_size)
{
    size_t len = 0u;
    bool truncated = false;
    bool last_cr = false;
    bool done = false;
    bool result = false;

    if (dst == NULL || dst_size == 0u) {
        return false;
    }

    hal_irq_disable();
    if (lines > 0u) {
        while (!done) {
            uint8_t c = buf[tail];

            tail = (tail + 1u) & BUF_MASK;
            count--;
            if (c == (uint8_t)'\n') {
                lines--;
                done = true;
            } else {
                last_cr = (c == (uint8_t)'\r');
                if (len < dst_size - 1u) {
                    dst[len] = (char)c;
                    len++;
                } else {
                    truncated = true;
                }
            }
        }
        if (last_cr && !truncated) {
            len--;
        }
        dst[len] = '\0';
        result = true;
    }
    hal_irq_enable();

    return result;
}

size_t uart_rx_available(void)
{
    return count;
}

uint32_t uart_rx_dropped(void)
{
    return dropped;
}