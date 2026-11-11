#include "parser.h"

#include <stddef.h>


parse_status_t parse_command(const char *line, command_t *out)
{
    if (line == NULL || out == NULL) {
        return PARSE_ERR_NULL;
    }
    size_t len=strlen(line);
    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
        len--;
    }
    if (len == 0) {
        return PARSE_ERR_EMPTY;
    }
    const char *eq = NULL;
    for (size_t i = 0; i < len; i++) {
        if (line[i] == '=') {
            eq = &line[i];
            break;
        }
    }
    if (line[0] < 'A' || line[0] > 'Z') {
        return PARSE_ERR_BAD_KEY;
    }
    if (eq == NULL) {
        return PARSE_ERR_NO_EQUALS;
    }
    size_t key_len = eq - line;
    if (key_len > PARSE_KEY_MAX) {
        return PARSE_ERR_KEY_TOO_LONG;
    }
    for (size_t i = 0; i < key_len; i++) {
        char c = line[i];
        if (!(c >= 'A' && c <= 'Z') && !(c >= '0' && c <= '9') && c != '_') {
            return PARSE_ERR_BAD_KEY;
        }
    }
   const char *val_str = eq + 1;
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
    int64_t parsed_val = 0; 
    for (; idx < val_len; idx++) {
        if (val_str[idx] < '0' || val_str[idx] > '9') {
            return PARSE_ERR_BAD_VALUE;
        }
        parsed_val = parsed_val * 10 + (val_str[idx] - '0');
        if (!is_negative && parsed_val > PARSE_VALUE_MAX) {
            return PARSE_ERR_RANGE;
        }
        if (is_negative && (-parsed_val) < PARSE_VALUE_MIN) {
            return PARSE_ERR_RANGE;
        }
    }
    int32_t final_val = is_negative ? (int32_t)(-parsed_val) : (int32_t)parsed_val;
        for (size_t i = 0; i < key_len; i++) {
        out->key[i] = line[i];
    }
    out->key[key_len] = '\0';
    out->value = final_val;
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
        default: return "UNKNOWN";
    }
}
