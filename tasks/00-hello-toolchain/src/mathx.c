#include "mathx.h"

int square(int x)
{
    //(void)x; /* TODO: remove this line and implement */
    return x*x;
}

int clamp(int x, int lo, int hi)
{
    //(void)x; /* TODO: remove these lines and implement */
    //(void)lo;
    //(void)hi;
    int result = x;
    if (x < lo)
        result = lo;
    if (x > hi)
        result = hi;
    return result;
}