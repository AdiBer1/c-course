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

static int is_key_char(char c)
{
    return is_upper(c) || is_digit(c) || c == '_';
}

parse_status_t parse_command(const char *line, command_t *out)
{
    size_t len = 0;
    size_t i = 0;
    size_t j;
    int32_t limit;
    int32_t mag = 0;
    int neg = 0;

    if (line == NULL || out == NULL) {
        return PARSE_ERR_NULL;
    }

    while (line[len] != '\0') {
        len++;
    }
    /* ignore one line ending at the end */
    if (len >= 2 && line[len - 2] == '\r' && line[len - 1] == '\n') {
        len -= 2;
    } else if (len >= 1 && line[len - 1] == '\n') {
        len -= 1;
    }
    if (len == 0) {
        return PARSE_ERR_EMPTY;
    }

    /* key: up to the first '=' */
    while (i < len && line[i] != '=') {
        if (!is_key_char(line[i]) || (i == 0 && !is_upper(line[i]))) {
            return PARSE_ERR_BAD_KEY;
        }
        i++;
    }
    if (i == 0) {
        return PARSE_ERR_BAD_KEY; /* '=' is the first char */
    }
    if (i == len) {
        return PARSE_ERR_NO_EQUALS;
    }
    if (i > PARSE_KEY_MAX) {
        return PARSE_ERR_KEY_TOO_LONG;
    }

    /* value: check the format first */
    j = i + 1;
    if (j < len && line[j] == '-') {
        neg = 1;
        j++;
    }
    if (j == len) {
        return PARSE_ERR_BAD_VALUE;
    }
    for (size_t k = j; k < len; k++) {
        if (!is_digit(line[k])) {
            return PARSE_ERR_BAD_VALUE;
        }
    }

    /* then the range, checked before every step so int32_t never overflows */
    limit = neg ? -PARSE_VALUE_MIN : PARSE_VALUE_MAX;
    for (size_t k = j; k < len; k++) {
        int32_t d = line[k] - '0';
        if (mag > (limit - d) / 10) {
            return PARSE_ERR_RANGE;
        }
        mag = mag * 10 + d;
    }

    /* only now change *out */
    for (size_t k = 0; k < i; k++) {
        out->key[k] = line[k];
    }
    out->key[i] = '\0';
    out->value = neg ? -mag : mag;
    return PARSE_OK;
}

const char *parse_status_str(parse_status_t status)
{
    switch (status) {
    case PARSE_OK:
        return "PARSE_OK";
    case PARSE_ERR_NULL:
        return "PARSE_ERR_NULL";
    case PARSE_ERR_EMPTY:
        return "PARSE_ERR_EMPTY";
    case PARSE_ERR_NO_EQUALS:
        return "PARSE_ERR_NO_EQUALS";
    case PARSE_ERR_BAD_KEY:
        return "PARSE_ERR_BAD_KEY";
    case PARSE_ERR_KEY_TOO_LONG:
        return "PARSE_ERR_KEY_TOO_LONG";
    case PARSE_ERR_BAD_VALUE:
        return "PARSE_ERR_BAD_VALUE";
    case PARSE_ERR_RANGE:
        return "PARSE_ERR_RANGE";
    default:
        return "UNKNOWN";
    }
}