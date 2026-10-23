#include "uart_rx.h"

#include "sim_uart.h"

/*
 * TODO:
 *   - static storage: the buffer, indices/count, the "lines ready" counter,
 *     the dropped counter. Think about which ones need `volatile`.
 *   - implement every function; protect the main-loop side with
 *     hal_irq_disable() / hal_irq_enable()
 */

void uart_rx_init(void)
{
}

void uart_rx_isr(void)
{
}

bool uart_rx_line_ready(void)
{
    return false;
}

bool uart_rx_read_line(char *dst, size_t dst_size)
{
    (void)dst;
    (void)dst_size;
    return false;
}

size_t uart_rx_available(void)
{
    return 0;
}

uint32_t uart_rx_dropped(void)
{
    return 0;
}
