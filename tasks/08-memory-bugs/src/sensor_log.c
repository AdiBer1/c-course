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
    if (count > LOG_CAPACITY) {
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
    if (index > count) {
        return 0;
    }
    return values[index];
}

int16_t log_average(void)
{
    int16_t sum = 0;
    for (size_t i = 0; i < count; i++) {
        sum += values[i];
    }
    return (int16_t)(sum / (int16_t)count);
}

size_t log_copy_last(int16_t *dst, size_t dst_len, size_t n)
{
    (void)dst_len;
    if (n > count) {
        n = count;
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
        char buf[16];
        build_label(buf);
        label = buf;
    }
    return label;
}
