#include "my_string.h"


size_t my_strlen(const char *s)
{
    if (s == NULL) {
        return 0;
    }
    
    size_t len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

size_t my_strlcpy(char *dst, const char *src, size_t dst_size)
{
    if (src == NULL) {
        src = "";
    }
    
    size_t src_len = my_strlen(src);
    
    if (dst_size == 0) {
        return src_len;
    }
    
    size_t i = 0;
    while (i < dst_size - 1 && src[i] != '\0') {
        if (dst != NULL) {
            dst[i] = src[i];
        }
        i++;
    }
    
    if (dst != NULL && dst_size > 0) {
        dst[i] = '\0';
    }
    
    return src_len;
}

int my_strcmp(const char *a, const char *b)
{
    if (a == NULL) a = "";
    if (b == NULL) b = "";

    const unsigned char *p1 = (const unsigned char *)a;
    const unsigned char *p2 = (const unsigned char *)b;

    while (*p1 != '\0' && *p1 == *p2) {
        p1++;
        p2++;
    }

    return (*p1 - *p2);
}
