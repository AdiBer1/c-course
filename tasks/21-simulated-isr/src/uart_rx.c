#include "uart_rx.h"
#include "sim_uart.h"

// Shared state variables must be volatile because they are updated by both the ISR and main loop
static volatile uint8_t rx_buffer[UART_RX_BUF_SIZE];
static volatile size_t rx_head = 0;
static volatile size_t rx_tail = 0;
static volatile size_t rx_count = 0;
static volatile uint32_t lines_ready_count = 0;
static volatile uint32_t dropped_count = 0;

void uart_rx_init(void)
{
    hal_irq_disable();
    rx_head = 0;
    rx_tail = 0;
    rx_count = 0;
    lines_ready_count = 0;
    dropped_count = 0;
    hal_uart_attach_isr(uart_rx_isr);
    hal_irq_enable();
}

void uart_rx_isr(void)
{
    // CRITICAL: Always read the data register to clear the hardware interrupt flag
    uint8_t byte = hal_uart_read_dr();

    if (rx_count >= UART_RX_BUF_SIZE) {
        dropped_count++;
        return;
    }

    // Push byte into the circular ring buffer
    rx_buffer[rx_head] = byte;
    rx_head = (rx_head + 1) % UART_RX_BUF_SIZE;
    rx_count++;

    if (byte == '\n') {
        lines_ready_count++;
    }
}

bool uart_rx_line_ready(void)
{
    hal_irq_disable();
    bool ready = (lines_ready_count > 0);
    hal_irq_enable();
    return ready;
}

bool uart_rx_read_line(char *dst, size_t dst_size)
{
    if (dst == NULL || dst_size == 0) {
        return false;
    }

    // Critical section protecting read-modify-write of shared buffer state
    hal_irq_disable();
    
    if (lines_ready_count == 0) {
        hal_irq_enable();
        return false;
    }

    size_t search_tail = rx_tail;
    size_t bytes_to_examine = 0;
    bool found = false;

    for (size_t i = 0; i < rx_count; i++) {
        bytes_to_examine++;
        if (rx_buffer[search_tail] == '\n') {
            found = true;
            break;
        }
        search_tail = (search_tail + 1) % UART_RX_BUF_SIZE;
    }

    if (!found) {
        hal_irq_enable();
        return false;
    }

    lines_ready_count--;

    size_t dest_idx = 0;
    bool truncate = (dst_size == 1);

    for (size_t i = 0; i < bytes_to_examine; i++) {
        uint8_t b = rx_buffer[rx_tail];
        
        // If it's a carriage return ('\r') and the very next byte in the buffer is a newline ('\n'), skip it
        if (b == '\r') {
            size_t next_idx = (rx_tail + 1) % UART_RX_BUF_SIZE;
            if (rx_buffer[next_idx] == '\n') {
                rx_tail = (rx_tail + 1) % UART_RX_BUF_SIZE;
                rx_count--;
                continue;
            }
        }

        rx_tail = (rx_tail + 1) % UART_RX_BUF_SIZE;
        rx_count--;

        // If it's a newline, we drop it (end of line reached)
        if (b == '\n') {
            continue;
        }

        if (!truncate) {
            if (dest_idx + 1 < dst_size) {
                dst[dest_idx++] = (char)b;
            } else {
                truncate = true;
            }
        }
    }

    dst[dest_idx] = '\0';
    hal_irq_enable();
    return true;
}

size_t uart_rx_available(void)
{
    hal_irq_disable();
    size_t count = rx_count;
    hal_irq_enable();
    return count;
}

uint32_t uart_rx_dropped(void)
{
    hal_irq_disable();
    uint32_t dropped = dropped_count;
    hal_irq_enable();
    return dropped;
}