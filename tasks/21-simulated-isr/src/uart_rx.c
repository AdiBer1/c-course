#include "uart_rx.h"

#include "sim_uart.h"

#define UART_RX_MASK (UART_RX_BUF_SIZE - 1u)

/*
 * Shared between the ISR and the main loop, so all volatile:
 *   head   - written only by the ISR
 *   tail   - written only by the main loop
 *   count  - ISR does ++, main loop does --  (critical section in main loop)
 *   lines  - ISR does ++, main loop does --  (critical section in main loop)
 *   dropped - written only by the ISR
 */
static volatile uint8_t rx_buf[UART_RX_BUF_SIZE];
static volatile size_t rx_head;
static volatile size_t rx_tail;
static volatile size_t rx_count;
static volatile uint32_t rx_lines;
static volatile uint32_t rx_dropped;

void uart_rx_init(void)
{
    hal_irq_disable();
    rx_head = 0u;
    rx_tail = 0u;
    rx_count = 0u;
    rx_lines = 0u;
    rx_dropped = 0u;
    hal_irq_enable();

    hal_uart_attach_isr(uart_rx_isr);
}

void uart_rx_isr(void)
{
    /* always read the data register, even if we can't keep the byte */
    uint8_t byte = hal_uart_read_dr();

    if (rx_count >= UART_RX_BUF_SIZE) {
        rx_dropped++;
        return;
    }

    rx_buf[rx_head] = byte;
    rx_head = (rx_head + 1u) & UART_RX_MASK;
    rx_count++;

    if (byte == (uint8_t)'\n') {
        rx_lines++;
    }
}

bool uart_rx_line_ready(void)
{
    return rx_lines > 0u;
}

/* Main loop only: take one byte. The caller knows the buffer is not empty. */
static uint8_t pop_byte(void)
{
    /* the ISR never writes the slot at tail while it holds data, so no lock here */
    uint8_t byte = rx_buf[rx_tail];
    rx_tail = (rx_tail + 1u) & UART_RX_MASK;

    /* count-- is read-modify-write on shared data: protect it */
    hal_irq_disable();
    rx_count--;
    hal_irq_enable();

    return byte;
}

bool uart_rx_read_line(char *dst, size_t dst_size)
{
    size_t n = 0u;
    bool last_stored_cr = false;

    if (dst == NULL || dst_size == 0u) {
        return false;
    }
    if (rx_lines == 0u) {
        return false;
    }

    /* a complete line is buffered, so a '\n' will come */
    while (rx_count > 0u) {
        uint8_t byte = pop_byte();

        if (byte == (uint8_t)'\n') {
            break;
        }

        if (n < dst_size - 1u) {
            dst[n] = (char)byte;
            n++;
            last_stored_cr = (byte == (uint8_t)'\r');
        } else {
            /* line too long: discard the rest of it */
            last_stored_cr = false;
        }
    }

    /* drop a '\r' that came right before the '\n' */
    if (last_stored_cr) {
        n--;
    }
    dst[n] = '\0';

    hal_irq_disable();
    rx_lines--;
    hal_irq_enable();

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