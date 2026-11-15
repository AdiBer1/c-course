#include "parser.h"
#include <stddef.h>
#include <stdint.h>

static int is_valid_key_char(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

parse_status_t parse_command(const char *line, command_t *out)
{
    if (!line || !out)
        return PARSE_ERR_NULL;

    size_t len = 0;
    while (line[len] != '\0')
        len++;

    /* Strip one optional trailing "\r\n" or "\n" */
    if (len >= 2 && line[len - 2] == '\r' && line[len - 1] == '\n')
        len -= 2;
    else if (len >= 1 && line[len - 1] == '\n')
        len -= 1;

    if (len == 0)
        return PARSE_ERR_EMPTY;

    /* Find first '=' */
    size_t eq = 0;
    while (eq < len && line[eq] != '=')
        eq++;

    /* Step 3a: Check bad key characters / empty key / non-letter start BEFORE no-equals and length */
    if (eq == 0 || !(line[0] >= 'A' && line[0] <= 'Z'))
        return PARSE_ERR_BAD_KEY;

    for (size_t j = 0; j < eq; j++) {
        if (!is_valid_key_char(line[j]))
            return PARSE_ERR_BAD_KEY;
    }

    /* Step 3b: No '=' found */
    if (eq == len)
        return PARSE_ERR_NO_EQUALS;

    /* Step 3c: Key length */
    if (eq > PARSE_KEY_MAX)
        return PARSE_ERR_KEY_TOO_LONG;

    /* Step 4: Value syntax */
    size_t k = eq + 1;
    int32_t sign = 1;
    if (k < len && line[k] == '-') {
        sign = -1;
        k++;
    }
    if (k == len)
        return PARSE_ERR_BAD_VALUE;

    for (size_t j = k; j < len; j++) {
        if (line[j] < '0' || line[j] > '9')
            return PARSE_ERR_BAD_VALUE;
    }

    /* Step 5: Value conversion & range check without int32_t overflow */
    int32_t value = 0;
    for (size_t j = k; j < len; j++) {
        int32_t digit = line[j] - '0';
        if (sign == 1) {
            if (value > (PARSE_VALUE_MAX - digit) / 10)
                return PARSE_ERR_RANGE;
            value = value * 10 + digit;
        } else {
            if (value < (PARSE_VALUE_MIN + digit) / 10)
                return PARSE_ERR_RANGE;
            value = value * 10 - digit;
        }
    }

    command_t temp;
    for (size_t j = 0; j < eq; j++)
        temp.key[j] = line[j];
    temp.key[eq] = '\0';
    temp.value = value;
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