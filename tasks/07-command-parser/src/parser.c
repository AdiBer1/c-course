#include "parser.h"

#include <stddef.h>

/* ---------------------------------------------------------------- helpers */

static int is_upper(char c)
{
    return c >= 'A' && c <= 'Z';
}

static int is_digit(char c)
{
    return c >= '0' && c <= '9';
}

/* Allowed in a key: A-Z, 0-9, '_'. */
static int is_key_char(char c)
{
    return is_upper(c) || is_digit(c) || c == '_';
}

static size_t text_length(const char *s)
{
    size_t n = 0u;
    while (s[n] != '\0') {
        n++;
    }
    return n;
}

/* ------------------------------------------------------------------- API */

parse_status_t parse_command(const char *line, command_t *out)
{
    /* 1. NULL pointers */
    if (line == NULL || out == NULL) {
        return PARSE_ERR_NULL;
    }

    /* 2. Remove ONE optional line ending at the very end: "\r\n" or "\n". */
    size_t len = text_length(line);
    if (len >= 2u && line[len - 2u] == '\r' && line[len - 1u] == '\n') {
        len -= 2u;
    } else if (len >= 1u && line[len - 1u] == '\n') {
        len -= 1u;
    }
    if (len == 0u) {
        return PARSE_ERR_EMPTY;
    }

    /* 3. The key: from the start up to the first '='.
     *    Only scan here; nothing is written to *out yet. */
    size_t i = 0u;
    while (i < len && line[i] != '=') {
        if (i == 0u && !is_upper(line[i])) {
            return PARSE_ERR_BAD_KEY; /* must start with a letter A-Z */
        }
        if (!is_key_char(line[i])) {
            return PARSE_ERR_BAD_KEY; /* e.g. a space or lowercase letter */
        }
        i++;
    }
    if (i == len) {
        return PARSE_ERR_NO_EQUALS; /* reached the end without any '=' */
    }
    if (i == 0u) {
        return PARSE_ERR_BAD_KEY; /* '=' is the very first char: empty key */
    }
    size_t key_len = i;
    if (key_len > (size_t)PARSE_KEY_MAX) {
        return PARSE_ERR_KEY_TOO_LONG;
    }

    /* 4. The value: everything after the '='. Syntax is checked over the
     *    WHOLE value before the range, so "X=99999999x" is BAD_VALUE. */
    size_t pos = key_len + 1u; /* first char after '=' */
    int negative = 0;
    if (pos < len && line[pos] == '-') {
        negative = 1;
        pos++;
    }
    if (pos == len) {
        return PARSE_ERR_BAD_VALUE; /* empty value or "-" alone */
    }

    /* Overflow-safe accumulation: as soon as the magnitude passes the limit
     * we stop adding digits and only remember "too big". Before each step
     * magnitude <= 1000000, so magnitude * 10 + 9 <= 10000009: it can never
     * overflow int32_t, no matter how many digits follow. */
    int32_t magnitude = 0;
    int too_big = 0;
    for (; pos < len; pos++) {
        char c = line[pos];
        if (!is_digit(c)) {
            return PARSE_ERR_BAD_VALUE; /* non-digit or junk after the number */
        }
        if (!too_big) {
            magnitude = magnitude * 10 + (int32_t)(c - '0');
            if (magnitude > PARSE_VALUE_MAX) {
                too_big = 1;
            }
        }
    }

    /* 5. Range. PARSE_VALUE_MIN == -PARSE_VALUE_MAX, so one limit is enough. */
    if (too_big) {
        return PARSE_ERR_RANGE;
    }
    int32_t value = negative ? -magnitude : magnitude;
    if (value < PARSE_VALUE_MIN || value > PARSE_VALUE_MAX) {
        return PARSE_ERR_RANGE;
    }

    /* Everything is valid: only now write to *out. */
    for (size_t k = 0u; k < key_len; k++) {
        out->key[k] = line[k];
    }
    out->key[key_len] = '\0';
    out->value = value;
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