#include "parser.h"

#include <stddef.h>

/* TODO: implement both functions. Small static helper functions are welcome. */

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

    /* 1. NULL */
    if (line == NULL || out == NULL) {
        return PARSE_ERR_NULL;
    }

    /* 2. length without the optional line ending */
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

        /* 3. key: from the start up to the first '=' */
    while (i < len && line[i] != '=') {
        if (i == 0 && !is_upper(line[i])) {
            return PARSE_ERR_BAD_KEY;
        }
        if (!is_key_char(line[i])) {
            return PARSE_ERR_BAD_KEY;
        }
        i++;
    }
    if (i == 0) {
        return PARSE_ERR_BAD_KEY;
    }
    if (i == len) {
        return PARSE_ERR_NO_EQUALS;
    }
    if (i > PARSE_KEY_MAX) {
        return PARSE_ERR_KEY_TOO_LONG;
    }
    i++; /* skip '=' */

    /* TODO: value (next step) */
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
    }
    return "UNKNOWN";
}