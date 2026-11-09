#include "parser.h"

#include <stddef.h>

static int is_upper(char c) { return c >= 'A' && c <= 'Z'; }
static int is_digit(char c) { return c >= '0' && c <= '9'; }
static int is_key_char(char c) { return is_upper(c) || is_digit(c) || c == '_'; }

parse_status_t parse_command(const char *line, command_t *out)
{
    if (!line || !out) return PARSE_ERR_NULL;

    /* strip trailing \r\n - only strip \n or \r\n, not standalone \r */
    const char *p = line;
    size_t len = 0;
    while (p[len]) len++;
    if (len > 0 && p[len-1] == '\n') {
        len--;
        if (len > 0 && p[len-1] == '\r') len--;
    }

    if (len == 0) return PARSE_ERR_EMPTY;

    /* parse key */
    size_t i = 0;
    if (i >= len || p[i] == '=') return PARSE_ERR_BAD_KEY;
    if (!is_upper(p[i])) return PARSE_ERR_BAD_KEY;

    size_t key_start = i;
    int key_too_long = 0;
    while (i < len && p[i] != '=') {
        if (!is_key_char(p[i])) return PARSE_ERR_BAD_KEY;
        i++;
        if (i - key_start > PARSE_KEY_MAX) key_too_long = 1;
    }
    if (i >= len) return PARSE_ERR_NO_EQUALS;
    if (key_too_long) return PARSE_ERR_KEY_TOO_LONG;
    size_t key_len = i - key_start;
    i++; /* skip '=' */

    /* parse value */
    if (i >= len) return PARSE_ERR_BAD_VALUE;
    int negative = 0;
    if (p[i] == '-') { negative = 1; i++; }
    if (i >= len || !is_digit(p[i])) return PARSE_ERR_BAD_VALUE;

    int32_t val = 0;
    int range_err = 0;
    while (i < len) {
        if (!is_digit(p[i])) return PARSE_ERR_BAD_VALUE;
        int d = p[i] - '0';
        if (!range_err) {
            if (val > (PARSE_VALUE_MAX - d) / 10)
                range_err = 1;
            else
                val = val * 10 + d;
        }
        i++;
    }
    if (range_err) return PARSE_ERR_RANGE;
    if (negative) val = -val;
    if (val < PARSE_VALUE_MIN || val > PARSE_VALUE_MAX) return PARSE_ERR_RANGE;

    /* commit */
    size_t k;
    for (k = 0; k < key_len; k++) out->key[k] = p[key_start + k];
    out->key[k] = '\0';
    out->value = val;
    return PARSE_OK;
}

const char *parse_status_str(parse_status_t status)
{
    switch (status) {
        case PARSE_OK:             return "PARSE_OK";
        case PARSE_ERR_NULL:       return "PARSE_ERR_NULL";
        case PARSE_ERR_EMPTY:      return "PARSE_ERR_EMPTY";
        case PARSE_ERR_NO_EQUALS:  return "PARSE_ERR_NO_EQUALS";
        case PARSE_ERR_BAD_KEY:    return "PARSE_ERR_BAD_KEY";
        case PARSE_ERR_KEY_TOO_LONG: return "PARSE_ERR_KEY_TOO_LONG";
        case PARSE_ERR_BAD_VALUE:  return "PARSE_ERR_BAD_VALUE";
        case PARSE_ERR_RANGE:      return "PARSE_ERR_RANGE";
        default:                   return "UNKNOWN";
    }
}
