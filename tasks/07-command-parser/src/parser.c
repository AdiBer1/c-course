#include "parser.h"

#include <stddef.h>

/* TODO: implement both functions. Small static helper functions are welcome. */

parse_status_t parse_command(const char *line, command_t *out)
{
    size_t len = 0;

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

    /* TODO: key and value (next steps) */
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