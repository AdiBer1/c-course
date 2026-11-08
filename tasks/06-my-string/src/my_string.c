#include "my_string.h"

size_t my_strlen(const char *s)
{
    size_t n = 0;

    if (s == NULL) {
        return 0;
    }
    while (s[n] != '\0') {
        n++;
    }
    return n;
}

size_t my_strlcpy(char *dst, const char *src, size_t dst_size)
{
    size_t src_len;
    size_t copy_len;
    size_t i;

    if (src == NULL) {
        src = "";
    }

    src_len = my_strlen(src);

    if (dst == NULL || dst_size == 0) {
        return src_len;
    }

    /* Leave room for the terminating '\0'. */
    copy_len = (src_len < dst_size - 1) ? src_len : dst_size - 1;

    for (i = 0; i < copy_len; i++) {
        dst[i] = src[i];
    }
    dst[copy_len] = '\0';

    return src_len;
}

int my_strcmp(const char *a, const char *b)
{
    const unsigned char *ua;
    const unsigned char *ub;

    if (a == NULL) {
        a = "";
    }
    if (b == NULL) {
        b = "";
    }

    ua = (const unsigned char *)a;
    ub = (const unsigned char *)b;

    while (*ua != '\0' && *ua == *ub) {
        ua++;
        ub++;
    }
    return (int)*ua - (int)*ub;
}