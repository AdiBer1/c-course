#include "mathx.h"

int square(int x)
{
    (void)x;
    return     x=x*x; 

}

int clamp(int x, int lo, int hi)
{
    (void)x; 
    (void)lo;
    (void)hi;
    if (x < lo) {
        return lo;
    } else if (x > hi) {
        return hi;
    } else {
        return x;
    }
    return 0;
}
