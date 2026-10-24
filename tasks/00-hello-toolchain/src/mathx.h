/*
 * mathx.h - two tiny integer helpers.
 *
 * This header is the module's public interface: it says WHAT the module
 * offers. mathx.c says HOW. Any .c file that wants to call these functions
 * includes this header so the compiler knows their signatures.
 *
 * Do not change this file.
 */
#ifndef MATHX_H /* include guard: the body is read only once per .c file */
#define MATHX_H

/* Return x * x. Example: square(7) == 49, square(-3) == 9. */
int square(int x);

/*
 * Limit x to the range [lo, hi] (lo <= hi):
 *   x < lo  -> lo
 *   x > hi  -> hi
 *   else    -> x
 * Example: clamp(150, 0, 100) == 100.
 */
int clamp(int x, int lo, int hi);

#endif /* MATHX_H */
