#include "max3.h"

int max3(int a, int b, int c)
{
    if (a>=b &&a>=c)
        return a ;
    if (b>=c&&b>=a)
        return b;
    return c;
}
