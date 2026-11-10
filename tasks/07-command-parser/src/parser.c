
#include "parser.h"

#include <stddef.h>

static int is_letter(char c)
{
    return c >= 'A' && c <= 'Z';
}

static int is_digit(char c)
{
    return c >= '0' && c <= '9';
}

static int is_key_char(char c)
{
    return is_letter(c) || is_digit(c) || c == '_';
}

parse_status_t parse_command(const char *line, command_t *out)
{
    size_t length = 0;
    size_t key_length = 0;
    size_t i = 0;
    int negative = 0;
    int out_of_range = 0;
    int32_t value = 0;
    int32_t limit;
    command_t temp;

    if (line == NULL || out == NULL)
    {
        return PARSE_ERR_NULL;
    }

    while (line[length] != '\0')
    {
        length++;
    }

    if (length > 0 && line[length - 1] == '\n')
    {
        length--;

        if (length > 0 && line[length - 1] == '\r')
        {
            length--;
        }
    }

    if (length == 0)
    {
        return PARSE_ERR_EMPTY;
    }

    while (i < length && line[i] != '=')
    {
        if (!is_key_char(line[i]))
        {
            return PARSE_ERR_BAD_KEY;
        }

        if (i == 0 && !is_letter(line[i]))
        {
            return PARSE_ERR_BAD_KEY;
        }

        i++;
    }

    if (i == 0)
    {
        return PARSE_ERR_BAD_KEY;
    }

    if (i == length)
    {
        return PARSE_ERR_NO_EQUALS;
    }

    key_length = i;

    if (key_length > PARSE_KEY_MAX)
    {
        return PARSE_ERR_KEY_TOO_LONG;
    }

    for (i = 0; i < key_length; i++)
    {
        temp.key[i] = line[i];
    }

    temp.key[key_length] = '\0';

    i = key_length + 1;

    if (i >= length)
    {
        return PARSE_ERR_BAD_VALUE;
    }

    if (line[i] == '-')
    {
        negative = 1;
        i++;
    }

    if (i >= length || !is_digit(line[i]))
    {
        return PARSE_ERR_BAD_VALUE;
    }

    limit = negative ? -PARSE_VALUE_MIN : PARSE_VALUE_MAX;

    for (; i < length; i++)
    {
        int32_t digit;

        if (!is_digit(line[i]))
        {
            return PARSE_ERR_BAD_VALUE;
        }

        digit = (int32_t)(line[i] - '0');

        if (!out_of_range)
        {
            if (value > (limit - digit) / 10)
            {
                out_of_range = 1;
            }
            else
            {
                value = value * 10 + digit;
            }
        }
    }

    if (out_of_range)
    {
        return PARSE_ERR_RANGE;
    }

    if (negative)
    {
        value = -value;
    }

    temp.value = value;
    *out = temp;

    return PARSE_OK;
}

const char *parse_status_str(parse_status_t status)
{
    switch (status)
    {
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
