#include "my_string.h"

/* TODO: implement every function. No <string.h>! */

size_t my_strlen(const char *s)
{
    if (s == NULL)
    {
        return 0;
    }

    size_t len = 0;

    while (s[len] != '\0')
    {
        len++;
    }

    return len;
}

/* Copy a string safely into a destination buffer. */
size_t my_strlcpy(char *dst, const char *src, size_t dst_size)
{
    size_t src_len = my_strlen(src);

    if (dst == NULL || dst_size == 0)
    {
        return src_len;
    }

    if (src == NULL)
    {
        dst[0] = '\0';
        return 0;
    }

    size_t i = 0;

    while (i < dst_size - 1 && src[i] != '\0')
    {
        dst[i] = src[i];
        i++;
    }

    dst[i] = '\0';

    return src_len;
}

/* Compare two strings. */
int my_strcmp(const char *a, const char *b)
{
    if (a == NULL)
    {
        a = "";
    }

    if (b == NULL)
    {
        b = "";
    }

    size_t i = 0;

    while ((unsigned char)a[i] == (unsigned char)b[i])
    {
        if (a[i] == '\0')
        {
            return 0;
        }

        i++;
    }

    return (unsigned char)a[i] - (unsigned char)b[i];
}
