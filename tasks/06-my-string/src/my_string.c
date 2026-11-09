#include "my_string.h"

size_t my_strlen(const char *s)
{
    if (!s) return 0;
    size_t n = 0;
    while (s[n]) n++;
    return n;
}

size_t my_strlcpy(char *dst, const char *src, size_t dst_size)
{
    const char *s = src ? src : "";
    size_t src_len = my_strlen(s);
    if (dst && dst_size > 0) {
        size_t copy = src_len < dst_size - 1 ? src_len : dst_size - 1;
        size_t i;
        for (i = 0; i < copy; i++) dst[i] = s[i];
        dst[i] = '\0';
    }
    return src_len;
}

int my_strcmp(const char *a, const char *b)
{
    const unsigned char *ua = (const unsigned char *)(a ? a : "");
    const unsigned char *ub = (const unsigned char *)(b ? b : "");
    while (*ua && *ua == *ub) { ua++; ub++; }
    return (int)*ua - (int)*ub;
}
