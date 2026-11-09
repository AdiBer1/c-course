#include "max3.h"

int max3(int a, int b, int c)
{
    int m = a > b ? a : b;
    return m > c ? m : c;
}
