#include "uart_rx.h"

#include "sim_uart.h"

/*
 * Who writes what:
 *   buf[]    ISR writes (push), main loop reads (pop)
 *   head     ISR only
 *   tail     main loop only
 *   count    BOTH  -> volatile, and a critical section on the main side
 *   lines    BOTH  -> volatile, and a critical section on the main side
 *   dropped  ISR writes, main loop reads -> volatile
 * Everything the ISR touches is volatile, so the main loop always re-reads
 * the real value from memory instead of a stale copy in a register.
 */
static volatile uint8_t buf[UART_RX_BUF_SIZE];
static volatile size_t head;  /* next free slot (ISR) */
static volatile size_t tail;  /* oldest byte (main loop) */
static volatile size_t count; /* bytes buffered */
static volatile uint32_t lines;   /* complete lines ('\n') buffered */
static volatile uint32_t dropped; /* bytes lost because the buffer was full */

/* UART_RX_BUF_SIZE is a power of two, so wrapping is a single AND. */
#define WRAP(i) ((i) & (UART_RX_BUF_SIZE - 1u))

void uart_rx_init(void)
{
    hal_irq_disable();
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
    /* ALWAYS read the data register first: on real hardware this is what
     * clears the RX interrupt, otherwise it would fire again forever. */
    uint8_t byte = (uint8_t)hal_uart_read_dr();

    if (count >= UART_RX_BUF_SIZE) {
        dropped++; /* buffer full: the byte is lost, but counted */
        return;
    }

    buf[head] = byte;
    head = WRAP(head + 1u);
    count++;
    if (byte == (uint8_t)'\n') {
        lines++; /* tell the main loop a complete line is ready */
    }
    /* Short and never waits: all line parsing happens in the main loop. */
}

bool uart_rx_line_ready(void)
{
    return lines > 0u; /* one read of a volatile: no critical section needed */
}

bool uart_rx_read_line(char *dst, size_t dst_size)
{
    if (dst == NULL || dst_size == 0u) {
        return false;
    }

    /* CRITICAL SECTION: count and lines are also changed by the ISR, and
     * count-- / lines-- are read-modify-write operations. With interrupts
     * off, the ISR cannot run halfway through them. Kept short: at most
     * UART_RX_BUF_SIZE bytes are moved. Every path re-enables interrupts. */
    hal_irq_disable();

    if (lines == 0u) {
        hal_irq_enable();
        return false; /* no complete line: nothing changes */
    }

    size_t n = 0u;            /* chars stored in dst */
    uint8_t prev = 0u;        /* previous byte of the line */
    bool prev_stored = false; /* was that previous byte stored in dst? */

    while (count > 0u) {
        uint8_t byte = buf[tail];
        tail = WRAP(tail + 1u);
        count--;

        if (byte == (uint8_t)'\n') {
            /* Drop a '\r' right before the '\n' (Windows-style "\r\n"). */
            if (prev == (uint8_t)'\r' && prev_stored) {
                n--;
            }
            break;
        }
        if (n < dst_size - 1u) {
            dst[n] = (char)byte;
            n++;
            prev_stored = true;
        } else {
            prev_stored = false; /* line too long: the rest is discarded */
        }
        prev = byte;
    }
    lines--;

    hal_irq_enable();

    dst[n] = '\0';
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