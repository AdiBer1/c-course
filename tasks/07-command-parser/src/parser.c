#include "parser.h"

#include <stddef.h>

static int is_upper(char c)
{
    return (c >= 'A' && c <= 'Z');
}

static int is_digit(char c)
{
    return (c >= '0' && c <= '9');
}

parse_status_t parse_command(const char *line, command_t *out)
{
    size_t len = 0;
    size_t i;
    size_t key_len;
    int negative = 0;
    int too_big = 0;
    int32_t magnitude = 0;

    if (line == NULL || out == NULL) {
        return PARSE_ERR_NULL;
    }

    /* Length of the line, without a line ending at the very end. */
    while (line[len] != '\0') {
        len++;
    }
    if (len >= 1 && line[len - 1] == '\n') {
        len--;
        if (len >= 1 && line[len - 1] == '\r') {
            len--;
        }
    }
    if (len == 0) {
        return PARSE_ERR_EMPTY;
    }

    /* Key: read up to the first '='. */
    for (i = 0; i < len; i++) {
        char c = line[i];

        if (c == '=') {
            break;
        }
        if (!(is_upper(c) || (i > 0 && (is_digit(c) || c == '_')))) {
            return PARSE_ERR_BAD_KEY;
        }
    }
    if (i == len) {
        return PARSE_ERR_NO_EQUALS;
    }
    if (i == 0) {
        return PARSE_ERR_BAD_KEY;
    }
    key_len = i;
    if (key_len > PARSE_KEY_MAX) {
        return PARSE_ERR_KEY_TOO_LONG;
    }

    /* Value: [ '-' ] digit { digit } */
    i = key_len + 1;
    if (i < len && line[i] == '-') {
        negative = 1;
        i++;
    }
    if (i >= len) {
        return PARSE_ERR_BAD_VALUE;
    }
    for (; i < len; i++) {
        char c = line[i];

        if (!is_digit(c)) {
            return PARSE_ERR_BAD_VALUE;
        }
        if (!too_big) {
            magnitude = magnitude * 10 + (c - '0');
            if (magnitude > PARSE_VALUE_MAX) {
                too_big = 1;
            }
        }
    }
    if (too_big) {
        return PARSE_ERR_RANGE;
    }

    /* Everything is valid: only now write the output. */
    for (i = 0; i < key_len; i++) {
        out->key[i] = line[i];
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