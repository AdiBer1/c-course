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
    size_t src_len = my_strlen(src);
    size_t i = 0;

    if (dst == NULL || dst_size == 0) {
        return src_len;
    }
    if (src != NULL) {
        while (i < dst_size - 1 && src[i] != '\0') {
            dst[i] = src[i];
            i++;
        }
    }
    dst[i] = '\0';
    return src_len;
}

int my_strcmp(const char *a, const char *b)
{
    const unsigned char *pa = (const unsigned char *)(a != NULL ? a : "");
    const unsigned char *pb = (const unsigned char *)(b != NULL ? b : "");

    while (*pa != '\0' && *pa == *pb) {
        pa++;
        pb++;
    }
    return (int)*pa - (int)*pb;
}