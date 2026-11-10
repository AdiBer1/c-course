
#include "parser.h"

#include <stddef.h>

parse_status_t parse_command(const char *line, command_t *out)
{
    if (line == NULL || out == NULL) {
        return PARSE_ERR_NULL;
    }

    size_t len = 0;
    while (line[len] != '\0') {
        len++;
    }

    if (len > 0 && line[len - 1] == '\n') {
        len--;
        if (len > 0 && line[len - 1] == '\r') {
            len--;
        }
    }

    if (len == 0) {
        return PARSE_ERR_EMPTY;
    }

    size_t key_len = 0;

    while (key_len < len && line[key_len] != '=') {
        char c = line[key_len];

        if (key_len == 0) {
            if (c < 'A' || c > 'Z') {
                return PARSE_ERR_BAD_KEY;
            }
        } else {
            if (!((c >= 'A' && c <= 'Z') ||
                  (c >= '0' && c <= '9') ||
                  c == '_')) {
                return PARSE_ERR_BAD_KEY;
            }
        }

        key_len++;
    }

    if (key_len == 0) {
    return PARSE_ERR_BAD_KEY;
    }

    if (key_len == len) {
        return PARSE_ERR_NO_EQUALS;
    }

    if (key_len > PARSE_KEY_MAX) {
        return PARSE_ERR_KEY_TOO_LONG;
    }

    size_t value_start = key_len + 1;

    if (value_start == len) {
        return PARSE_ERR_BAD_VALUE;
    }

    int negative = 0;

    if (line[value_start] == '-') {
        negative = 1;
        value_start++;
    }

    if (value_start == len) {
        return PARSE_ERR_BAD_VALUE;
    }

    unsigned int magnitude = 0;
    unsigned int limit = negative
        ? (unsigned int)(-PARSE_VALUE_MIN)
        : (unsigned int)PARSE_VALUE_MAX;

    int out_of_range = 0;

    for (size_t i = value_start; i < len; i++) {
        char c = line[i];

        if (c < '0' || c > '9') {
            return PARSE_ERR_BAD_VALUE;
        }

        unsigned int digit = (unsigned int)(c - '0');

        if (magnitude > (limit - digit) / 10) {
            out_of_range = 1;
        } else if (!out_of_range) {
            magnitude = magnitude * 10 + digit;
        }
    }

    if (out_of_range) {
        return PARSE_ERR_RANGE;
    }

    command_t temp;

    for (size_t i = 0; i < key_len; i++) {
        temp.key[i] = line[i];
    }

    temp.key[key_len] = '\0';

    temp.value = negative
        ? -(int32_t)magnitude
        : (int32_t)magnitude;

    *out = temp;

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
