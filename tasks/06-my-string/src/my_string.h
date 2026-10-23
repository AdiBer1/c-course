/*
 * my_string.h - your own versions of three basic string functions.
 *
 * A C string is an array of char ending with a '\0' (NUL) byte. Nothing
 * stores its length: functions walk the bytes until they find the '\0'.
 * A string without its '\0', or a copy into a too-small buffer, is the
 * classic way firmware gets a buffer overflow.
 *
 * Constraints: do not call any function from <string.h> (strlen, strcpy,
 * strncpy, strcmp, memcpy, ...). Write the loops yourself.
 *
 * Do not change this file.
 */
#ifndef MY_STRING_H
#define MY_STRING_H

#include <stddef.h>

/* Number of chars before the terminating '\0'. my_strlen(NULL) == 0. */
size_t my_strlen(const char *s);

/*
 * Bounded copy (like BSD strlcpy):
 *   - copies at most dst_size - 1 chars of src into dst
 *   - ALWAYS terminates dst with '\0' when dst_size > 0
 *   - never writes dst[dst_size] or beyond
 *   - returns my_strlen(src), so the caller can detect truncation:
 *         if (my_strlcpy(buf, s, sizeof buf) >= sizeof buf) { truncated }
 * If dst_size == 0, nothing is written. If src is NULL it counts as "".
 * If dst is NULL, nothing is written and the return value is still my_strlen(src).
 */
size_t my_strlcpy(char *dst, const char *src, size_t dst_size);

/*
 * Compare two strings like strcmp:
 *   < 0 if a sorts before b, 0 if equal, > 0 if a sorts after b.
 * Characters are compared as unsigned char values. NULL counts as "".
 */
int my_strcmp(const char *a, const char *b);

#endif /* MY_STRING_H */
