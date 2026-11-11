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

    if (len >= 2 && line[len - 2] == '\r' && line[len - 1] == '\n') {
        len -= 2;
    } else if (len >= 1 && line[len - 1] == '\n') {
        len -= 1;
    }

    if (len == 0) {
        return PARSE_ERR_EMPTY;
    }

    size_t eq_pos = 0;
    int has_equals = 0;

    for (size_t i = 0; i < len; i++) {
        char c = line[i];
        if (c == '=') {
            eq_pos = i;
            has_equals = 1;
            break;
        }
        if (!((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')) {
            return PARSE_ERR_BAD_KEY;
        }
    }

    if (!has_equals) {
        return PARSE_ERR_NO_EQUALS;
    }

    if (eq_pos == 0) {
        return PARSE_ERR_BAD_KEY;
    }

    if (line[0] < 'A' || line[0] > 'Z') {
        return PARSE_ERR_BAD_KEY;
    }

    if (eq_pos > PARSE_KEY_MAX) {
        return PARSE_ERR_KEY_TOO_LONG;
    }

    size_t val_start = eq_pos + 1;
    if (val_start >= len) {
        return PARSE_ERR_BAD_VALUE;
    }

    size_t i = val_start;
    int negative = 0;

    if (line[i] == '-') {
        negative = 1;
        i++;
    }

    if (i >= len) {
        return PARSE_ERR_BAD_VALUE;
    }

    size_t digits_count = 0;
    int32_t accum = 0;
    int out_of_range = 0;

    while (i < len) {
        char c = line[i];
        if (c < '0' || c > '9') {
            return PARSE_ERR_BAD_VALUE;
        }

        int32_t digit = c - '0';

        if (!out_of_range) {
            if (negative) {
                if (accum < (PARSE_VALUE_MIN + digit) / 10) {
                    out_of_range = 1;
                } else {
                    accum = accum * 10 - digit;
                }
            } else {
                if (accum > (PARSE_VALUE_MAX - digit) / 10) {
                    out_of_range = 1;
                } else {
                    accum = accum * 10 + digit;
                }
            }
        }

        digits_count++;
        i++;
    }

    if (digits_count == 0) {
        return PARSE_ERR_BAD_VALUE;
    }

    if (out_of_range || accum < PARSE_VALUE_MIN || accum > PARSE_VALUE_MAX) {
        return PARSE_ERR_RANGE;
    }

    for (size_t k = 0; k < eq_pos; k++) {
        out->key[k] = line[k];
    }
    out->key[eq_pos] = '\0';
    out->value = accum;

    return PARSE_OK;
}

const char *parse_status_str(parse_status_t status)
{
    switch (status) {
        case PARSE_OK:              return "PARSE_OK";
        case PARSE_ERR_NULL:        return "PARSE_ERR_NULL";
        case PARSE_ERR_EMPTY:       return "PARSE_ERR_EMPTY";
        case PARSE_ERR_NO_EQUALS:   return "PARSE_ERR_NO_EQUALS";
        case PARSE_ERR_BAD_KEY:     return "PARSE_ERR_BAD_KEY";
        case PARSE_ERR_KEY_TOO_LONG:return "PARSE_ERR_KEY_TOO_LONG";
        case PARSE_ERR_BAD_VALUE:   return "PARSE_ERR_BAD_VALUE";
        case PARSE_ERR_RANGE:       return "PARSE_ERR_RANGE";
        default:                    return "UNKNOWN";
    }
}