#include "my_string.h"

size_t my_strlen(const char *s)
{
    size_t len = 0u;

    if (s == NULL) {
        return 0u;
    }
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

size_t my_strlcpy(char *dst, const char *src, size_t dst_size)
{
    if (src == NULL) {
        src = ""; /* NULL counts as the empty string */
    }

    size_t src_len = my_strlen(src);

    /* Nothing may be written: no buffer, or a buffer of size 0.
     * Checking dst_size == 0 first also avoids (size_t)0 - 1, a huge number. */
    if (dst == NULL || dst_size == 0u) {
        return src_len;
    }

    /* At most dst_size - 1 chars: the last byte is reserved for '\0'. */
    size_t n = (src_len < dst_size - 1u) ? src_len : dst_size - 1u;

    for (size_t i = 0u; i < n; i++) {
        dst[i] = src[i];
    }
    dst[n] = '\0'; /* n <= dst_size - 1, so this never goes past the buffer */

    /* Return the full source length so the caller can detect truncation. */
    return src_len;
}

int my_strcmp(const char *a, const char *b)
{
    if (a == NULL) {
        a = "";
    }
    if (b == NULL) {
        b = "";
    }

    /* Compare as unsigned char, so bytes >= 0x80 sort after ASCII. */
    const unsigned char *ua = (const unsigned char *)a;
    const unsigned char *ub = (const unsigned char *)b;

    while (*ua != '\0' && *ua == *ub) {
        ua++;
        ub++;
    }
    return (int)*ua - (int)*ub;
}