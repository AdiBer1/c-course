/*
 * uart_rx.h - interrupt-driven UART line receiver.
 *
 * The classic split between interrupt context and the main loop:
 *
 *   uart_rx_isr()        runs in INTERRUPT context, once per received byte.
 *                        Must be short: read the byte, store it, set a flag.
 *                        It can interrupt the main loop at ANY instruction.
 *
 *   uart_rx_read_line()  runs in the MAIN LOOP. Takes a complete line out of
 *                        the buffer when one is there.
 *
 *       byte arrives --> [ ISR ] --push--> ring buffer --pop--> [ main loop ]
 *                           |                                        ^
 *                           +--- sets "line ready" (volatile) -------+
 *
 * Data shared between the two must be:
 *   - declared `volatile`, so the compiler re-reads it every time instead of
 *     keeping a stale copy in a register
 *   - protected in the main loop by a CRITICAL SECTION
 *     (hal_irq_disable() ... hal_irq_enable()) wherever the main loop does a
 *     read-modify-write of shared state, so the ISR cannot run halfway through
 *
 * The hardware is simulated by hal/sim_uart.h: use hal_uart_read_dr(),
 * hal_irq_disable(), hal_irq_enable() and hal_uart_attach_isr(), exactly as
 * you would use the real ones on a microcontroller.
 *
 * Constraints: no malloc; all state is static inside uart_rx.c.
 *
 * Do not change this file.
 */
#ifndef UART_RX_H
#define UART_RX_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Bytes the receive buffer can hold. A power of two. */
#define UART_RX_BUF_SIZE 16u

/*
 * Empty the buffer, reset the counters and flags, and register uart_rx_isr
 * as the RX interrupt handler (hal_uart_attach_isr).
 */
void uart_rx_init(void);

/*
 * The RX interrupt handler. ALWAYS reads the byte from the data register
 * (hal_uart_read_dr), even when it cannot keep it: on real hardware the
 * interrupt keeps firing until the register is read.
 *   - buffer full  -> the byte is dropped and counted (uart_rx_dropped)
 *   - otherwise    -> the byte is stored; if it is '\n', one more complete
 *                     line is available
 */
void uart_rx_isr(void);

/* true if at least one complete line ('\n'-terminated) is buffered. */
bool uart_rx_line_ready(void);

/*
 * Take the oldest complete line out of the buffer.
 * Returns false (and changes nothing) if no complete line is buffered, or if
 * dst is NULL or dst_size is 0.
 * Otherwise removes the line including its '\n' from the buffer, copies it
 * into dst WITHOUT the '\n' (and without a '\r' right before it), always
 * '\0'-terminated. If the line does not fit, it is truncated to dst_size - 1
 * chars and the rest of that line is discarded. Returns true.
 *
 * A line must fit in the buffer including its '\n'; bytes that arrive while
 * the buffer is full are lost.
 */
bool uart_rx_read_line(char *dst, size_t dst_size);

/* Number of bytes currently buffered. */
size_t uart_rx_available(void);

/* Bytes dropped because the buffer was full, since init. */
uint32_t uart_rx_dropped(void);

#endif /* UART_RX_H */
