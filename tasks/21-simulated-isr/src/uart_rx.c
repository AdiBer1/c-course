#include "uart_rx.h"

#include "sim_uart.h"

static volatile uint8_t buf[UART_RX_BUF_SIZE];
static volatile size_t head;
static volatile size_t tail;
static volatile size_t count;
static volatile uint32_t lines_ready;
static volatile uint32_t dropped;

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
    if (byte == '\n') {
        lines_ready++;
    }
}

void uart_rx_init(void)
{
    hal_irq_disable();
    head = 0;
    tail = 0;
    count = 0;
    lines_ready = 0;
    dropped = 0;
    hal_irq_enable();
    hal_uart_attach_isr(uart_rx_isr);
}

bool uart_rx_line_ready(void)
{
    hal_irq_disable();
    bool ready = lines_ready > 0;
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

    size_t n = 0;
    while (n < count && buf[(tail + n) % UART_RX_BUF_SIZE] != '\n') {
        n++;
    }

    size_t text_len = n;
    if (text_len > 0 && buf[(tail + text_len - 1u) % UART_RX_BUF_SIZE] == '\r') {
        text_len--;
    }

    size_t copy = (text_len < dst_size - 1u) ? text_len : dst_size - 1u;
    for (size_t k = 0; k < copy; k++) {
        dst[k] = (char)buf[(tail + k) % UART_RX_BUF_SIZE];
    }
    dst[copy] = '\0';

    tail = (tail + n + 1u) % UART_RX_BUF_SIZE;
    count -= n + 1u;
    lines_ready--;
    hal_irq_enable();
    return true;
}

size_t uart_rx_available(void)
{
    hal_irq_disable();
    size_t n = count;
    hal_irq_enable();
    return n;
}

uint32_t uart_rx_dropped(void)
{
    hal_irq_disable();
    uint32_t d = dropped;
    hal_irq_enable();
    return d;
}