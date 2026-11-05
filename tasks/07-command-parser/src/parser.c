#include "parser.h"
#include <stddef.h>

static int is_upper(char c) {
    return c >= 'A' && c <= 'Z';
}

static int is_digit(char c) {
    return c >= '0' && c <= '9';
}

static int is_valid_key_char(char c) {
    return is_upper(c) || is_digit(c) || c == '_';
}

parse_status_t parse_command(const char *line, command_t *out)
{
    if (!line || !out) {
        return PARSE_ERR_NULL;
    }

    int len = 0;
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

    int i = 0;
    
    while (i < len && line[i] != '=') {
        if (i == 0) {
            if (!is_upper(line[i])) {
                return PARSE_ERR_BAD_KEY;
            }
        } else {
            if (!is_valid_key_char(line[i])) {
                return PARSE_ERR_BAD_KEY;
            }
        }
        i++;
    }

    if (i == len) {
        return PARSE_ERR_NO_EQUALS;
    }
    
    int key_len = i;
    if (key_len == 0) {
        return PARSE_ERR_BAD_KEY;
    }
    if (key_len > PARSE_KEY_MAX) {
        return PARSE_ERR_KEY_TOO_LONG;
    }

    i++;
    int val_start = i;
    
    if (val_start == len) {
        return PARSE_ERR_BAD_VALUE; 
    }

    int is_negative = 0;
    if (line[i] == '-') {
        is_negative = 1;
        i++;
    }

    if (i == len) {
        return PARSE_ERR_BAD_VALUE; 
    }

    for (int j = i; j < len; j++) {
        if (!is_digit(line[j])) {
            return PARSE_ERR_BAD_VALUE;
        }
    }

    int32_t temp_val = 0;
    for (int j = i; j < len; j++) {
        int digit = line[j] - '0';
        
        if (temp_val > PARSE_VALUE_MAX) {
            temp_val = PARSE_VALUE_MAX + 1;
        } else {
            temp_val = temp_val * 10 + digit;
        }
    }

    if (is_negative) {
        temp_val = -temp_val;
    }

    if (temp_val < PARSE_VALUE_MIN || temp_val > PARSE_VALUE_MAX) {
        return PARSE_ERR_RANGE;
    }

    for (int k = 0; k < key_len; k++) {
        out->key[k] = line[k];
    }
    out->key[key_len] = '\0';
    out->value = temp_val;

    return PARSE_OK;
}

const char *parse_status_str(parse_status_t status)
{
    switch (status) {
        case PARSE_OK:             return "PARSE_OK";
        case PARSE_ERR_NULL:       return "PARSE_ERR_NULL";
        case PARSE_ERR_EMPTY:      return "PARSE_ERR_EMPTY";
        case PARSE_ERR_NO_EQUALS:  return "PARSE_ERR_NO_EQUALS";
        case PARSE_ERR_BAD_KEY:    return "PARSE_ERR_BAD_KEY";
        case PARSE_ERR_KEY_TOO_LONG: return "PARSE_ERR_KEY_TOO_LONG";
        case PARSE_ERR_BAD_VALUE:  return "PARSE_ERR_BAD_VALUE";
        case PARSE_ERR_RANGE:      return "PARSE_ERR_RANGE";
        default:                   return "UNKNOWN";
    }
}