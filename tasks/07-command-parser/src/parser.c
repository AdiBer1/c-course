#include "parser.h"

#include <stddef.h>

/* TODO: implement both functions. Small static helper functions are welcome. */

/* Check if a character is an uppercase letter. */
static int is_uppercase(char c)
{
    return c >= 'A' && c <= 'Z';
}

/* Check if a character is a digit. */
static int is_digit(char c)
{
    return c >= '0' && c <= '9';
}

/* Check if a character is allowed in a key. */
static int is_valid_key_char(char c)
{
    return is_uppercase(c) || is_digit(c) || c == '_';
}

parse_status_t parse_command(const char *line, command_t *out)
{
    if (line == NULL || out == NULL)
    {
        return PARSE_ERR_NULL;
    }

    /* Find the length of the input line. */
    size_t len = 0;

    while (line[len] != '\0')
    {
        len++;
    }

    /* Remove one optional line ending. */
    if (len > 0 && line[len - 1] == '\n')
    {
        len--;

        if (len > 0 && line[len - 1] == '\r')
        {
            len--;
        }
    }

    if (len == 0)
    {
        return PARSE_ERR_EMPTY;
    }

    /* Find '=' and validate the key. */
    size_t key_len = 0;
    int bad_key = 0;

    while (key_len < len && line[key_len] != '=')
    {
        if ((key_len == 0 && !is_uppercase(line[key_len])) ||
            (key_len > 0 && !is_valid_key_char(line[key_len])))
        {
            bad_key = 1;
        }

        key_len++;
    }

    if (key_len == len)
    {
        return bad_key ? PARSE_ERR_BAD_KEY : PARSE_ERR_NO_EQUALS;
    }

    if (bad_key || key_len == 0)
    {
        return PARSE_ERR_BAD_KEY;
    }

    if (key_len > PARSE_KEY_MAX)
    {
        return PARSE_ERR_KEY_TOO_LONG;
    }

    /* Parse the value. */
    size_t i = key_len + 1;

    if (i == len)
    {
        return PARSE_ERR_BAD_VALUE;
    }

    int negative = 0;

    if (line[i] == '-')
    {
        negative = 1;
        i++;
    }

    if (i == len)
    {
        return PARSE_ERR_BAD_VALUE;
    }

    uint32_t magnitude = 0;
    int out_of_range = 0;

    for (; i < len; i++)
    {
        if (!is_digit(line[i]))
        {
            return PARSE_ERR_BAD_VALUE;
        }

        uint32_t digit = (uint32_t)(line[i] - '0');

        /* Check before multiplying to prevent overflow. */
        if (!out_of_range)
        {
            if (magnitude > (1000000U - digit) / 10U)
            {
                out_of_range = 1;
            }
            else
            {
                magnitude = magnitude * 10U + digit;
            }
        }
    }

    if (out_of_range)
    {
        return PARSE_ERR_RANGE;
    }

    command_t result;

    for (size_t j = 0; j < key_len; j++)
    {
        result.key[j] = line[j];
    }

    result.key[key_len] = '\0';

    result.value = negative ? -(int32_t)magnitude : (int32_t)magnitude;

    /* Update output only after all checks succeed. */
    *out = result;

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
