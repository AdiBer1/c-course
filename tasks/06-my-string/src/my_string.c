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
        if (src == NULL) {
        src = "";
    }

    if (dst != NULL && dst_size > 0) {
        size_t place = 0;
        while (place + 1 < dst_size && *(src + place) != '\0')
        {
            dst[place] = src[place];
            place++;
        }
        dst[place] = '\0';
    }

    return my_strlen(src);
}

int my_strcmp(const char *a, const char *b)
{
   if (a == NULL) {
        a = "";
    }
    if (b == NULL) {
        b = "";
    }

    const unsigned char *una = (const unsigned char *)a;
    const unsigned char *unb = (const unsigned char *)b;

    while (*una == *unb && *una != '\0')
    {
        una++;
        unb++;
    }

    return (int)*una - (int)*unb;
}
