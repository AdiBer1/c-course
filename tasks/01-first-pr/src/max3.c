#include "max3.h"

int max3(int a, int b, int c)
{
    int best = (a > b) ? a : b;
    return (best > c) ? best : c;
}
