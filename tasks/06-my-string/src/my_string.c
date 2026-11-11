#include "my_string.h"

//count how much char has before /0
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
    size_t i = 0;
    size_t len;

    if (src == NULL) {
        src = "";
    }
    len = my_strlen(src);

    if (dst == NULL || dst_size == 0) {
        return len;
    }

    while (i < dst_size - 1 && src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';

    return len;
}

int my_strcmp(const char *a, const char *b)
{
    if (a == NULL) {
        a = "";
    }
    if (b == NULL) {
        b = "";
    }

    while (*a != '\0' && *a == *b) {
        a++;
        b++;
    }

    return (int)(unsigned char)*a - (int)(unsigned char)*b;
}
