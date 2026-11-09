#include "uart_rx.h"

#include "sim_uart.h"

static volatile uint8_t  buf[UART_RX_BUF_SIZE];
static volatile size_t   head;      /* ISR writes here */
static volatile size_t   tail;      /* main loop reads here */
static volatile size_t   count;     /* bytes in buffer */
static volatile uint32_t lines;     /* complete lines buffered */
static volatile uint32_t dropped;   /* bytes dropped (buffer full) */

void uart_rx_init(void)
{
    head    = 0;
    tail    = 0;
    count   = 0;
    lines   = 0;
    dropped = 0;
    hal_uart_attach_isr(uart_rx_isr);
}

void uart_rx_isr(void)
{
    uint8_t byte = hal_uart_read_dr();
    if (count >= UART_RX_BUF_SIZE) {
        dropped++;
        return;
    }
    buf[head] = byte;
    head = (head + 1u) % UART_RX_BUF_SIZE;
    count++;
    if (byte == '\n')
        lines++;
}

bool uart_rx_line_ready(void)
{
    return lines > 0;
}

bool uart_rx_read_line(char *dst, size_t dst_size)
{
    if (!dst || dst_size == 0) return false;

    hal_irq_disable();
    bool ready = (lines > 0);
    hal_irq_enable();

    if (!ready) return false;

    size_t out = 0;
    bool   found_newline = false;

    while (!found_newline) {
        hal_irq_disable();
        size_t avail = count;
        hal_irq_enable();

        if (avail == 0) break;

        uint8_t byte = (uint8_t)buf[tail];

        hal_irq_disable();
        tail  = (tail + 1u) % UART_RX_BUF_SIZE;
        count--;
        hal_irq_enable();

        if (byte == '\n') {
            found_newline = true;
            /* strip trailing \r if present */
            if (out > 0 && dst[out - 1] == '\r')
                out--;
        } else {
            if (out < dst_size - 1u)
                dst[out++] = (char)byte;
        }
    }

    dst[out] = '\0';

    hal_irq_disable();
    lines--;
    hal_irq_enable();

    return true;
}

size_t uart_rx_available(void)
{
    return count;
}

uint32_t uart_rx_dropped(void)
{
    return dropped;
}
