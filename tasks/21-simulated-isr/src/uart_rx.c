#include "uart_rx.h"

#include "sim_uart.h"

#define INDEX_MASK (UART_RX_BUF_SIZE - 1u)

/* State shared between the ISR and the main loop: volatile. */
static volatile uint8_t buf[UART_RX_BUF_SIZE];
static volatile size_t head;    /* ISR writes here */
static volatile size_t tail;    /* main loop reads here */
static volatile size_t count;   /* bytes stored (written by both sides) */
static volatile size_t lines;   /* complete lines stored (written by both) */
static volatile uint32_t dropped;

void uart_rx_isr(void)
{
    /* always read the data register, even if the byte is dropped */
    uint8_t byte = hal_uart_read_dr();

    if (count >= UART_RX_BUF_SIZE) {
        dropped++;
        return;
    }
    buf[head] = byte;
    head = (head + 1u) & INDEX_MASK;
    count++;
    if (byte == (uint8_t)'\n') {
        lines++;
    }
}

void uart_rx_init(void)
{
    head = 0u;
    tail = 0u;
    count = 0u;
    lines = 0u;
    dropped = 0u;

    /* register the handler last, after the state is ready */
    hal_uart_attach_isr(uart_rx_isr);
}

bool uart_rx_line_ready(void)
{
    return lines > 0u;
}

bool uart_rx_read_line(char *dst, size_t dst_size)
{
    size_t len = 0u;
    uint8_t last = 0u;
    bool last_stored = false;
    bool done = false;

    if (dst == NULL || dst_size == 0u) {
        return false;
    }
    /* Only the ISR increases `lines` and only we decrease it, so if it
     * is non-zero here it stays non-zero: safe to check without a lock. */
    if (lines == 0u) {
        return false;
    }

    hal_irq_disable(); /* critical section: we change count/tail/lines */
    while (!done && count > 0u) {
        uint8_t b = buf[tail];

        tail = (tail + 1u) & INDEX_MASK;
        count--;

        if (b == (uint8_t)'\n') {
            /* drop one '\r' right before the '\n' (if we kept it) */
            if (last == (uint8_t)'\r' && last_stored) {
                len--;
            }
            done = true;
        } else {
            if (len < dst_size - 1u) {
                dst[len] = (char)b;
                len++;
                last_stored = true;
            } else {
                last_stored = false; /* line too long: discard the rest */
            }
            last = b;
        }
    }
    if (done) {
        lines--;
    }
    hal_irq_enable(); /* a pending byte is handled right here */

    dst[len] = '\0';
    return done;
}

size_t uart_rx_available(void)
{
    return count;
}

uint32_t uart_rx_dropped(void)
{
    return dropped;
}