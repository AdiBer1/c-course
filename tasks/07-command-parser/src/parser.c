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
    /* 1. NULL checks */
    if (line == NULL || out == NULL) {
        return PARSE_ERR_NULL;
    }

    /* 2. Length without one optional "\n" or "\r\n" at the very end */
    size_t len = 0U;
    while (line[len] != '\0') {
        len++;
    }
    if (len > 0U && line[len - 1U] == '\n') {
        len--;
        if (len > 0U && line[len - 1U] == '\r') {
            len--;
        }
    }
    if (len == 0U) {
        return PARSE_ERR_EMPTY;
    }

    /* 3. Key: from the start up to the first '=' */
    size_t i = 0U;
    while (i < len && line[i] != '=') {
        if (!is_key_char(line[i]) || (i == 0U && !is_upper(line[i]))) {
            return PARSE_ERR_BAD_KEY;
        }
        i++;
    }
    if (i == len) {
        return PARSE_ERR_NO_EQUALS;
    }
    if (i == 0U) {
        return PARSE_ERR_BAD_KEY; /* '=' as the very first char */
    }
    size_t key_len = i;
    if (key_len > (size_t)PARSE_KEY_MAX) {
        return PARSE_ERR_KEY_TOO_LONG;
    }

    /* 4. Value syntax: [ '-' ] digit { digit } */
    size_t pos = key_len + 1U; /* first char after '=' */
    int negative = 0;
    if (pos < len && line[pos] == '-') {
        negative = 1;
        pos++;
    }
    if (pos == len) {
        return PARSE_ERR_BAD_VALUE; /* empty value or "-" alone */
    }

    /*
     * Accumulate as a positive magnitude. Once the magnitude is above the
     * limit we stop adding digits (so it can never overflow), but we keep
     * scanning so that junk after the number is still BAD_VALUE.
     */
    const int32_t limit = negative ? -(int32_t)PARSE_VALUE_MIN : (int32_t)PARSE_VALUE_MAX;
    int32_t magnitude = 0;
    int out_of_range = 0;
    for (size_t k = pos; k < len; k++) {
        if (!is_digit(line[k])) {
            return PARSE_ERR_BAD_VALUE;
        }
        if (!out_of_range) {
            int32_t digit = (int32_t)(line[k] - '0');
            /* magnitude * 10 + digit > limit, written so it cannot overflow */
            if (magnitude > (limit - digit) / 10) {
                out_of_range = 1;
            } else {
                magnitude = magnitude * 10 + digit;
            }
        }
    }

    /* 5. Range */
    if (out_of_range) {
        return PARSE_ERR_RANGE;
    }

    /* Success: only now write to *out */
    for (size_t k = 0U; k < key_len; k++) {
        out->key[k] = line[k];
    }
    out->key[key_len] = '\0';
    out->value = negative ? -magnitude : magnitude;
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
