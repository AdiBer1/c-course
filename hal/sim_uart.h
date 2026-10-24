/*
 * sim_uart.h - a simulated UART receiver with an RX interrupt.
 *
 * Two sides:
 *
 *   Firmware side (what your code may call, like on a real MCU):
 *     hal_uart_read_dr()   read the received byte from the data register
 *     hal_irq_disable()    enter a critical section (interrupts off)
 *     hal_irq_enable()     leave it (a pending interrupt runs right now)
 *
 *   Test side (plays the role of the outside world / the CPU):
 *     sim_uart_receive(b)  a byte arrives: it is put in the data register and
 *                          the RX interrupt fires. If interrupts are disabled
 *                          it stays pending and the ISR runs as soon as they
 *                          are enabled again. A second byte arriving while one
 *                          is still pending overwrites it (an "overrun").
 *
 * Like on real hardware, the ISR runs "in the middle of" whatever the main
 * code was doing; here that is simulated by calling it directly.
 */
#ifndef SIM_UART_H
#define SIM_UART_H

#include <stdbool.h>
#include <stdint.h>

typedef void (*isr_fn_t)(void);

/* ---- firmware side */
uint8_t hal_uart_read_dr(void);
void hal_irq_disable(void);
void hal_irq_enable(void);

/* Register the RX interrupt handler (like enabling it in the interrupt controller). */
void hal_uart_attach_isr(isr_fn_t isr);

/* ---- test side */
void sim_uart_reset(void);           /* detach ISR, enable interrupts, clear counters */
void sim_uart_receive(uint8_t byte); /* a byte arrives on the wire */
bool sim_irq_enabled(void);
uint32_t sim_irq_disable_calls(void); /* how often hal_irq_disable() was called */
uint32_t sim_uart_overruns(void);     /* bytes lost in the data register itself */
uint32_t sim_isr_runs(void);          /* how often the ISR ran */
uint32_t sim_isr_unread(void);        /* ISR runs that did not read the data register */

#endif /* SIM_UART_H */
