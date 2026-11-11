#include "parser.h"
#include <stddef.h>
#include <string.h>

parse_status_t parse_command(const char *line, command_t *out)
{
    if (line == NULL || out == NULL) {
        return PARSE_ERR_NULL;
    }
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        len--;
        if (len > 0 && line[len - 1] == '\r') {
            len--;
        }
    }
    if (len == 0) {
        return PARSE_ERR_EMPTY;
    }
    if (line[0] < 'A' || line[0] > 'Z') {
        return PARSE_ERR_BAD_KEY;
    }
    size_t eq_idx = 0;
    int found_equals = 0;
    for (size_t i = 0; i < len; i++) {
        if (line[i] == '=') {
            eq_idx = i;
            found_equals = 1;
            break;
        }
        char c = line[i];
        if (!(c >= 'A' && c <= 'Z') && !(c >= '0' && c <= '9') && c != '_') {
            return PARSE_ERR_BAD_KEY;
        }
    }
    if (!found_equals) {
        return PARSE_ERR_NO_EQUALS;
    }
    if (eq_idx > PARSE_KEY_MAX) {
        return PARSE_ERR_KEY_TOO_LONG;
    }
    const char *val_str = &line[eq_idx + 1];
    size_t val_len = (line + len) - val_str;
    if (val_len == 0) {
        return PARSE_ERR_BAD_VALUE;
    }
    int is_negative = 0;
    size_t idx = 0;
    if (val_str[0] == '-') {
        is_negative = 1;
        idx++;
        if (idx == val_len) {
            return PARSE_ERR_BAD_VALUE;
        }
    }
    for (size_t i = idx; i < val_len; i++) {
        if (val_str[i] < '0' || val_str[i] > '9') {
            return PARSE_ERR_BAD_VALUE;
        }
    }
    int64_t parsed_val = 0;
    for (size_t i = idx; i < val_len; i++) {
        parsed_val = parsed_val * 10 + (val_str[i] - '0');
        if (!is_negative && parsed_val > PARSE_VALUE_MAX) {
            return PARSE_ERR_RANGE;
        }
        if (is_negative && (-parsed_val) < PARSE_VALUE_MIN) {
            return PARSE_ERR_RANGE;
        }
    }
    int32_t final_val = is_negative ? (int32_t)(-parsed_val) : (int32_t)parsed_val;
    if (final_val < PARSE_VALUE_MIN || final_val > PARSE_VALUE_MAX) {
        return PARSE_ERR_RANGE;
    }
    for (size_t i = 0; i < eq_idx; i++) {
        out->key[i] = line[i];
    }
    out->key[eq_idx] = '\0';
    out->value = final_val;
    return PARSE_OK;
}

const char *parse_status_str(parse_status_t status)
{
    switch (status) {
        case PARSE_OK:              return "PARSE_OK";
        case PARSE_ERR_NULL:        return "PARSE_ERR_NULL";
        case PARSE_ERR_EMPTY:       return "PARSE_ERR_EMPTY";
        case PARSE_ERR_NO_EQUALS:   return "PARSE_ERR_NO_EQUALS";
        case PARSE_ERR_BAD_KEY:     return "PARSE_ERR_BAD_KEY";
        case PARSE_ERR_KEY_TOO_LONG:return "PARSE_ERR_KEY_TOO_LONG";
        case PARSE_ERR_BAD_VALUE:   return "PARSE_ERR_BAD_VALUE";
        case PARSE_ERR_RANGE:       return "PARSE_ERR_RANGE";
        default:                    return "UNKNOWN";
    }
}