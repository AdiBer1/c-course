#include "status_reg.h"

#include <stddef.h>

/* TODO: implement. Defining the field positions and widths as constants first will help. */

status_t status_decode(uint32_t reg)
{
    (void)reg;
    status_t s = {0};
    return s;
}

uint32_t status_encode(const status_t *s)
{
    (void)s;
    return 0;
}

status_t status_read(const volatile uint32_t *reg)
{
    (void)reg;
    status_t s = {0};
    return s;
}

int16_t status_temp_celsius(const status_t *s)
{
    (void)s;
    return 0;
}
