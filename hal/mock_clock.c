#include "mock_clock.h"

static uint32_t mock_now(void *ctx)
{
    mock_clock_t *mock = ctx;
    mock->reads++;
    return mock->now;
}

void mock_clock_init(mock_clock_t *mock, hal_clock_t *hal, uint32_t start_ms)
{
    mock->now = start_ms;
    mock->reads = 0;
    hal->now_ms = mock_now;
    hal->ctx = mock;
}

void mock_clock_advance(mock_clock_t *mock, uint32_t ms)
{
    mock->now += ms; /* unsigned: wraps around, exactly like real hardware */
}
