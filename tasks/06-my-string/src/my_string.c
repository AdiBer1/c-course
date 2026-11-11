#include "my_string.h"

size_t my_strlen(const char *s)
{
    if (s == NULL) {
        return 0;
    }
    size_t i = 0;
    while (s[i] != '\0') {
        i++;
    }
    return i;
}

size_t my_strlcpy(char *dst, const char *src, size_t dst_size)
{
    if (src == NULL) {
        if (dst != NULL && dst_size > 0) {
            dst[0] = '\0';
        }
        return 0;
    }

    if (dst == NULL) {
        return my_strlen(src);
    }
    
    if (dst_size > 0) {
        size_t i = 0;
        for (i = 0; i < dst_size - 1 && src[i] != '\0'; i++) {
            dst[i] = src[i];
        }
        dst[i] = '\0';
    }
    return my_strlen(src);
}

int my_strcmp(const char *a, const char *b)
{
    char empty = '\0';
    if (a == NULL) a = &empty;
    if (b == NULL) b = &empty;

    while (*a != '\0' && *a == *b) {
        a++;
        b++;
    }
    return (int)*(unsigned char *)a - (int)*(unsigned char *)b;
}