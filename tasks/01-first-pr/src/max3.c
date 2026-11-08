#include "max3.h"

int max3(int a, int b, int c)
{
    if (a >= b && a >= c)
        return a;

    if (c >= a && c >= b)
        return c;

    return b;
}