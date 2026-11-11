#include "mathx.h"

int square(int x)
{
    return x * x;
}

int clamp(int x, int lo, int hi)
{
    if (x < lo) {
        return lo;
    } else if (x > hi) {
        return hi;
    }
    return x;
}
