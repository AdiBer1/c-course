#include "uart_rx.h"

#include "sim_uart.h"

/* Shared between the ISR and the main loop, so all volatile. */
static volatile uint8_t rx_buf[UART_RX_BUF_SIZE]; /* the ring buffer */
static volatile size_t rx_head;
static volatile size_t rx_tail;
static volatile size_t rx_count;
static volatile uint32_t rx_lines;
static volatile uint32_t rx_dropped;

void uart_rx_init(void)
{
    /* Critical section: the ISR must not run while we reset its state. */
    hal_irq_disable();
    rx_head = 0;
    rx_tail = 0;
    rx_count = 0;
    rx_lines = 0;
    rx_dropped = 0;
    hal_irq_enable();

    hal_uart_attach_isr(uart_rx_isr);
}

void uart_rx_isr(void)
{
    uint8_t byte = hal_uart_read_dr();
    if (rx_count >= UART_RX_BUF_SIZE) {
        rx_dropped++; /* buffer full: drop the byte, but count it */
        return;
    }

    rx_buf[rx_head] = byte;
    rx_head = (rx_head + 1u) & (UART_RX_BUF_SIZE - 1u); /* wrap 15 -> 0 */
    rx_count++;

    if (byte == '\n') {
        rx_lines++; /* one more complete line is waiting */
    }
}

bool uart_rx_line_ready(void)
{
    return rx_lines > 0u;
}

bool uart_rx_read_line(char *dst, size_t dst_size)
{
    size_t n = 0;
    bool prev_cr = false; /* previous byte was '\r' and was stored */
    bool done = false;

    if (dst == NULL || dst_size == 0u) {
        return false;
    }

    hal_irq_disable();

    if (rx_lines == 0u) {
        hal_irq_enable();
        return false;
    }

    /* There is a '\n' in the buffer, so this loop always ends. */
    while (!done) {
        uint8_t c = rx_buf[rx_tail];
        rx_tail = (rx_tail + 1u) & (UART_RX_BUF_SIZE - 1u);
        rx_count--;

        if (c == '\n') {
            done = true;
        } else if (n < dst_size - 1u) {
            dst[n] = (char)c; /* fits: store it */
            n++;
            prev_cr = (c == '\r');
        } else {
            prev_cr = false; /* too long: discard the rest of the line */
        }
    }
    rx_lines--;

    hal_irq_enable();

    if (prev_cr) {
        n--; /* drop a '\r' right before the '\n' */
    }
    dst[n] = '\0';
    return true;
}

size_t uart_rx_available(void)
{
    return rx_count;
}

uint32_t uart_rx_dropped(void)
{
    return rx_dropped;
}