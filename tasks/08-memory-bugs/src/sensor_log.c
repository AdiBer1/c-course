/*
 * sensor_log.c - CONTAINS 6 BUGS. Find and fix them all.
 *
 * Most of them are memory bugs: the code compiles and "works" most of the
 * time, which is exactly what makes them dangerous. Use the tools:
 *   make test     AddressSanitizer / UBSan reports show the line and the
 *                 kind of bad access; the tests show what is wrong
 *   make score    the summary of what is still wrong
 *
 * Change as little as possible: this is a bug hunt, not a rewrite.
 */
#include "sensor_log.h"

static int16_t values[LOG_CAPACITY];
static size_t count;
static const char *label;

void log_init(void)
{
    count = 0;
}

bool log_add(int16_t value)
{
    /* FIX 1: was `count > LOG_CAPACITY`, which let count == 8 through
     * and wrote values[8], one past the end of the array. */
    if (count >= LOG_CAPACITY) {
        return false;
    }
    values[count] = value;
    count++;
    return true;
}

size_t log_count(void)
{
    return count;
}

int16_t log_get(size_t index)
{
    /* FIX 2: was `index > count`; index == count read a slot that holds
     * no reading (and values[8] when the log is full). */
    if (index >= count) {
        return 0;
    }
    return values[index];
}

int16_t log_average(void)
{
    /* FIX 4: was a division by zero when the log is empty. */
    if (count == 0u) {
        return 0;
    }
    /* FIX 3: was `int16_t sum`; 8 readings can add up to 8 * 32767 = 262136,
     * which does not fit in int16_t. int32_t holds any possible sum. */
    int32_t sum = 0;
    for (size_t i = 0; i < count; i++) {
        sum += values[i];
    }
    return (int16_t)(sum / (int32_t)count);
}

size_t log_copy_last(int16_t *dst, size_t dst_len, size_t n)
{
    if (n > count) {
        n = count;
    }
    /* FIX 5: dst_len was ignored (`(void)dst_len;`), so asking for more
     * readings than dst can hold wrote past the end of the caller's buffer. */
    if (n > dst_len) {
        n = dst_len;
    }
    size_t first = count - n;
    for (size_t i = 0; i < n; i++) {
        dst[i] = values[first + i];
    }
    return n;
}

/* Writes "sensor-log/<LOG_CAPACITY>" into buf (buf must hold 16 chars). */
static void build_label(char *buf)
{
    const char prefix[] = "sensor-log/";
    size_t i = 0;
    while (prefix[i] != '\0') {
        buf[i] = prefix[i];
        i++;
    }
    unsigned cap = LOG_CAPACITY;
    char digits[4];
    size_t n = 0;
    do {
        digits[n++] = (char)('0' + cap % 10u);
        cap /= 10u;
    } while (cap > 0u && n < sizeof digits);
    while (n > 0) {
        buf[i++] = digits[--n];
    }
    buf[i] = '\0';
}

const char *log_label(void)
{
    if (label == NULL) {
        /* FIX 6: buf was a local (stack) array, so `label` pointed to memory
         * that stopped existing when this function returned (dangling
         * pointer). `static` gives it the lifetime of the whole program. */
        static char buf[16];
        build_label(buf);
        label = buf;
    }
    return label;
}