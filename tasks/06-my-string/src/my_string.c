#include "my_string.h"

/* TODO: implement every function. No <string.h>! */

size_t my_strlen(const char *s)
{
    size_t counter = 0;
    if (s == NULL) {
        return 0;
    }
    while ((*s) != '\0') {
        counter++;
        s++;
    }
    return counter;
}

size_t my_strlcpy(char *dst, const char *src, size_t dst_size)
{
    size_t iterator = 0;
    if (src == NULL) {
        if (dst_size > 0 && dst != NULL) {
            dst[0] = '\0';
        }
        return 0;
    }
    size_t src_length = my_strlen(src);
    if (dst_size == 0 || dst == NULL) {
        return src_length;
    }
    while (iterator < dst_size - 1 && *src != '\0') {
        *dst++ = *src++;
        iterator++;
    }
    *dst = '\0';
    return src_length;
}

int my_strcmp(const char *a, const char *b)
{
    if (a == NULL) {
        a = "";
    }
    if (b == NULL) {
        b = "";
    }
    while (*a != '\0' && *b != '\0') {
        if ((unsigned char)*a != (unsigned char)*b) {
            return (unsigned char)*a - (unsigned char)*b;
        }
        a++;
        b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}
