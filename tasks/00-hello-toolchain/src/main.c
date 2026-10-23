/*
 * main.c - the program's entry point. Given; do not change.
 *
 * It only knows the functions through mathx.h. Their code lives in mathx.c,
 * and the linker joins the two object files into one program.
 */
#include <stdio.h>

#include "mathx.h"

int main(void)
{
    printf("Hello from C!\n");
    printf("square(7) = %d\n", square(7));
    printf("clamp(150, 0, 100) = %d\n", clamp(150, 0, 100));
    return 0;
}
