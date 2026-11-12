#include "my_string.h"

size_t my_strlen(const char *s)
{
    size_t len = 0;

    if (s == NULL) {
        return 0;
    }

    while (s[len] != '\0') {
        len++;
    }

    return len;
}

size_t my_strlcpy(char *dst, const char *src, size_t dst_size)
{
    size_t src_len = my_strlen(src);

    if (dst == NULL || dst_size == 0) {
        return src_len;
    }

    if (src == NULL) {
        dst[0] = '\0';
        return 0;
    }

    size_t i = 0;
    while (i < dst_size - 1 && src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }

    dst[i] = '\0';

    return src_len;
}

int my_strcmp(const char *a, const char *b)
{
    const char *s1 = (a != NULL) ? a : "";
    const char *s2 = (b != NULL) ? b : "";
    size_t i = 0;

    while (s1[i] != '\0' && s1[i] == s2[i]) {
        i++;
    }

    return (int)(unsigned char)s1[i] - (int)(unsigned char)s2[i];
}