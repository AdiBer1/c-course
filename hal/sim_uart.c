#include "sim_uart.h"

#include <stddef.h>

static isr_fn_t rx_isr;
static uint8_t data_register;
static bool rx_pending;
static bool irq_on = true;
static uint32_t disable_calls;
static uint32_t overruns;
static uint32_t isr_runs;
static uint32_t unread;

static void run_isr(void)
{
    if (rx_isr != NULL && rx_pending && irq_on) {
        isr_runs++;
        rx_isr();
        if (rx_pending) {
            /* On real hardware an ISR that never reads DR fires again forever. */
            unread++;
            rx_pending = false;
        }
    }
}

uint8_t hal_uart_read_dr(void)
{
    rx_pending = false;
    return data_register;
}

void hal_irq_disable(void)
{
    irq_on = false;
    disable_calls++;
}

void hal_irq_enable(void)
{
    irq_on = true;
    run_isr(); /* a pending interrupt is taken as soon as interrupts are on */
}

void hal_uart_attach_isr(isr_fn_t isr)
{
    rx_isr = isr;
}

void sim_uart_reset(void)
{
    rx_isr = NULL;
    data_register = 0;
    rx_pending = false;
    irq_on = true;
    disable_calls = 0;
    overruns = 0;
    isr_runs = 0;
    unread = 0;
}

void sim_uart_receive(uint8_t byte)
{
    if (rx_pending) {
        overruns++; /* previous byte never read: lost */
    }
    data_register = byte;
    rx_pending = true;
    run_isr();
}

bool sim_irq_enabled(void)
{
    return irq_on;
}

uint32_t sim_irq_disable_calls(void)
{
    return disable_calls;
}

uint32_t sim_uart_overruns(void)
{
    return overruns;
}

uint32_t sim_isr_runs(void)
{
    return isr_runs;
}

uint32_t sim_isr_unread(void)
{
    return unread;
}
