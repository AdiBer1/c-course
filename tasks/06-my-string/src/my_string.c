#include "my_string.h"


size_t my_strlen(const char *s)
{
    if (s == NULL) {
        return 0;
    }
    size_t size = 0;
    while (*s != '\0')
    {
        s = s + 1;
        size++;
    }
    return size;
}

size_t my_strlcpy(char *dst, const char *src, size_t dst_size)
{
    (void)dst;
    (void)src;
    (void)dst_size;
    return 0;
}

int my_strcmp(const char *a, const char *b)
{
    (void)a;
    (void)b;
    return 0;
}
