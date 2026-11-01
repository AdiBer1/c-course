#include "parser.h"

#include <stddef.h>

static int is_upper(char c)
{
    return c >= 'A' && c <= 'Z';
}

static int is_digit(char c)
{
    return c >= '0' && c <= '9';
}

parse_status_t parse_command(const char *line, command_t *out)
{
    size_t len = 0;
    size_t key_len = 0;
    size_t i;
    int negative = 0;
    int overflow = 0;
    int32_t limit;
    int32_t mag = 0;

    /* 1. NULL */
    if (line == NULL || out == NULL) {
        return PARSE_ERR_NULL;
    }

    /* 2. strip one optional line ending, then check for empty */
    while (line[len] != '\0') {
        len++;
    }
    if (len >= 2 && line[len - 2] == '\r' && line[len - 1] == '\n') {
        len -= 2;
    } else if (len >= 1 && line[len - 1] == '\n') {
        len -= 1;
    }
    if (len == 0) {
        return PARSE_ERR_EMPTY;
    }

    /* 3. key: scan up to the first '=' */
    while (key_len < len && line[key_len] != '=') {
        char c = line[key_len];
        int ok = (key_len == 0) ? is_upper(c)
                                : (is_upper(c) || is_digit(c) || c == '_');
        if (!ok) {
            return PARSE_ERR_BAD_KEY;
        }
        key_len++;
    }
    if (key_len == len) {
        return PARSE_ERR_NO_EQUALS;
    }
    if (key_len == 0) {
        return PARSE_ERR_BAD_KEY; /* '=' as the first char */
    }
    if (key_len > PARSE_KEY_MAX) {
        return PARSE_ERR_KEY_TOO_LONG;
    }

    /* 4 + 5. value: optional '-', then at least one digit */
    i = key_len + 1;
    if (i < len && line[i] == '-') {
        negative = 1;
        i++;
    }
    if (i >= len) {
        return PARSE_ERR_BAD_VALUE; /* empty value or "-" alone */
    }

    limit = negative ? -PARSE_VALUE_MIN : PARSE_VALUE_MAX;
    for (; i < len; i++) {
        int digit;

        if (!is_digit(line[i])) {
            return PARSE_ERR_BAD_VALUE;
        }
        digit = line[i] - '0';
        if (!overflow) {
            /* mag * 10 + digit <= limit, written so nothing can overflow */
            if (mag > (limit - digit) / 10) {
                overflow = 1;
            } else {
                mag = mag * 10 + digit;
            }
        }
    }
    if (overflow) {
        return PARSE_ERR_RANGE;
    }

    /* success: only now do we touch *out */
    for (i = 0; i < key_len; i++) {
        out->key[i] = line[i];
    }
    out->key[key_len] = '\0';
    out->value = negative ? -mag : mag;
    return PARSE_OK;
}

const char *parse_status_str(parse_status_t status)
{
    switch (status) {
    case PARSE_OK:               return "PARSE_OK";
    case PARSE_ERR_NULL:         return "PARSE_ERR_NULL";
    case PARSE_ERR_EMPTY:        return "PARSE_ERR_EMPTY";
    case PARSE_ERR_NO_EQUALS:    return "PARSE_ERR_NO_EQUALS";
    case PARSE_ERR_BAD_KEY:      return "PARSE_ERR_BAD_KEY";
    case PARSE_ERR_KEY_TOO_LONG: return "PARSE_ERR_KEY_TOO_LONG";
    case PARSE_ERR_BAD_VALUE:    return "PARSE_ERR_BAD_VALUE";
    case PARSE_ERR_RANGE:        return "PARSE_ERR_RANGE";
    default:                     return "UNKNOWN";
    }
}