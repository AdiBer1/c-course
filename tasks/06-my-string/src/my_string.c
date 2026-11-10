#include "my_string.h"

size_t my_strlen(const char *s)
{
    size_t length = 0;

    if (s == NULL)
    {
        return 0;
    }

    while (s[length] != '\0')
    {
        length++;
    }

    return length;
}

size_t my_strlcpy(char *dst, const char *src, size_t dst_size)
{
    size_t src_length = 0;
    size_t i = 0;

    if (src == NULL)
    {
        src = "";
    }

    while (src[src_length] != '\0')
    {
        src_length++;
    }

    if (dst == NULL || dst_size == 0)
    {
        return src_length;
    }

    while (i < dst_size - 1 && src[i] != '\0')
    {
        dst[i] = src[i];
        i++;
    }

    dst[i] = '\0';

    return src_length;
}

int my_strcmp(const char *a, const char *b)
{
    unsigned char char_a;
    unsigned char char_b;

    if (a == NULL)
    {
        a = "";
    }

    if (b == NULL)
    {
        b = "";
    }

    while (*a != '\0' && *b != '\0')
    {
        char_a = (unsigned char)*a;
        char_b = (unsigned char)*b;

        if (char_a != char_b)
        {
            return (int)char_a - (int)char_b;
        }

        a++;
        b++;
    }

    char_a = (unsigned char)*a;
    char_b = (unsigned char)*b;

    return (int)char_a - (int)char_b;
}