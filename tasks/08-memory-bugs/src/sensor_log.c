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
    if (index >= count) {
        return 0;
    }
    return values[index];
}

int16_t log_average(void)
{
    if (count == 0) {
        return 0;
    }
    int32_t sum = 0; /* 8 readings of up to 32767 need more than 16 bits */
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
        static char buf[16]; /* static: lives for the whole program */
        build_label(buf);
        label = buf;
    }
    return label;
}
