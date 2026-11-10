#include "max3.h"

int max3(int a, int b, int c)
{
    int largest = a;

    if (b > largest) {
        largest = b;
    }
    if (c > largest) {
        largest = c;
    }

    return largest;
}