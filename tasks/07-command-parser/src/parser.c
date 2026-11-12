#include "parser.h"
#include <stddef.h>

/* TODO: implement both functions. Small static helper functions are welcome. */

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

    size_t eq_idx = 0;
    int has_eq = 0;
    for (size_t i = 0; i < len; i++) {
        if (line[i] == '=') {
            eq_idx = i;
            has_eq = 1;
            break;
        }
    }

    size_t key_len = has_eq ? eq_idx : len;

    if (key_len == 0) {
        return PARSE_ERR_BAD_KEY;
    }

    if (line[0] < 'A' || line[0] > 'Z') {
        return PARSE_ERR_BAD_KEY;
    }

    for (size_t i = 0; i < key_len; i++) {
        char c = line[i];
        if (!((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')) {
            return PARSE_ERR_BAD_KEY;
        }
    }

    if (!has_eq) {
        return PARSE_ERR_NO_EQUALS;
    }

    if (key_len > PARSE_KEY_MAX) {
        return PARSE_ERR_KEY_TOO_LONG;
    }

    size_t val_start = eq_idx + 1;
    if (val_start >= len) {
        return PARSE_ERR_BAD_VALUE;
    }

    int negative = 0;
    size_t i = val_start;
    if (line[i] == '-') {
        negative = 1;
        i++;
    }

    if (i >= len) {
        return PARSE_ERR_BAD_VALUE;
    }

    int32_t val = 0;
    size_t digits_count = 0;

    while (i < len) {
        char c = line[i];
        if (c < '0' || c > '9') {
            return PARSE_ERR_BAD_VALUE;
        }
        int digit = c - '0';
        digits_count++;

        if (val > (INT32_MAX - digit) / 10) {
            return PARSE_ERR_RANGE;
        }
        val = val * 10 + digit;
        i++;
    }

    if (digits_count == 0) {
        return PARSE_ERR_BAD_VALUE;
    }

    int32_t final_val = 0;
    if (negative) {
        if (val > (int32_t)(-(PARSE_VALUE_MIN))) {
            return PARSE_ERR_RANGE;
        }
        final_val = -val;
    } else {
        final_val = val;
    }

    if (final_val < PARSE_VALUE_MIN || final_val > PARSE_VALUE_MAX) {
        return PARSE_ERR_RANGE;
    }

    for (size_t k = 0; k < key_len; k++) {
        out->key[k] = line[k];
    }
    out->key[key_len] = '\0';
    out->value = final_val;

    return PARSE_OK;
}