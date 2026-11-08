#include "parser.h"
#include <stddef.h>
#include <stdint.h>

parse_status_t parse_command(const char *line, command_t *out)
{
    if (!line || !out)
        return PARSE_ERR_NULL;

    uint8_t len = 0;
    while (line[len] != '\0') {
        if (line[len] == '\n') {
            if (line[len + 1] == '\0')
                break;
            return PARSE_ERR_BAD_VALUE;
        }
        if (line[len] == '\r') {
            if (line[len + 1] == '\n' && line[len + 2] == '\0')
                break;
            return PARSE_ERR_BAD_VALUE;
        }
        len++;
    }

    if (!len)
        return PARSE_ERR_EMPTY;

    uint8_t eq = 0;
    while (eq < len && line[eq] != '=')
        eq++;

    if (eq == len) {
        for (uint8_t j = 0; j < len; j++)
            if (!((line[j] >= 'A' && line[j] <= 'Z') || (line[j] >= '0' && line[j] <= '9')
                  || line[j] == '_'))
                return PARSE_ERR_BAD_KEY;
        return PARSE_ERR_NO_EQUALS;
    }

    if (!eq || !(line[0] >= 'A' && line[0] <= 'Z'))
        return PARSE_ERR_BAD_KEY;
    if (eq > PARSE_KEY_MAX)
        return PARSE_ERR_KEY_TOO_LONG;

    for (uint8_t j = 0; j < eq; j++)
        if (!((line[j] >= 'A' && line[j] <= 'Z') || (line[j] >= '0' && line[j] <= '9')
              || line[j] == '_'))
            return PARSE_ERR_BAD_KEY;

    uint8_t k = eq + 1;
    int sign = 1;
    if (k < len && line[k] == '-') {
        sign = -1;
        k++;
    }
    if (k == len)
        return PARSE_ERR_BAD_VALUE;

    for (uint8_t j = k; j < len; j++)
        if (line[j] < '0' || line[j] > '9')
            return PARSE_ERR_BAD_VALUE;

    int64_t value = 0;
    for (uint8_t j = k; j < len; j++) {
        value = value * 10 + (line[j] - '0');
        int64_t v = value * sign;
        if (v < PARSE_VALUE_MIN || v > PARSE_VALUE_MAX)
            return PARSE_ERR_RANGE;
    }

    command_t temp;
    for (uint8_t j = 0; j < eq; j++)
        temp.key[j] = line[j];
    temp.key[eq] = '\0';
    temp.value = (int32_t)(value * sign);
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
    }
    return "UNKNOWN";
}