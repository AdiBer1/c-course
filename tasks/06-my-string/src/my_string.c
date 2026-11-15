#include "my_string.h"

size_t my_strlen(const char *s)
{
    size_t len = 0U;
    if (s == NULL) {
        return 0U;
    }
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

size_t my_strlcpy(char *dst, const char *src, size_t dst_size)
{
    const char *source = (src == NULL) ? "" : src;
    size_t src_len = my_strlen(source);

    if (dst == NULL || dst_size == 0U) {
        return src_len;
    }

    size_t i = 0U;
    while (i < dst_size - 1U && source[i] != '\0') {
        dst[i] = source[i];
        i++;
    }
    dst[i] = '\0';

    return src_len;
}

int my_strcmp(const char *a, const char *b)
{
    const unsigned char *pa = (const unsigned char *)((a == NULL) ? "" : a);
    const unsigned char *pb = (const unsigned char *)((b == NULL) ? "" : b);

    while (*pa != '\0' && *pa == *pb) {
        pa++;
        pb++;
    }
    return (int)*pa - (int)*pb;
}
