/*
 * parser.h - parse "KEY=VALUE" command lines into a struct.
 *
 * Devices are often configured over a serial line with text commands:
 *
 *     LED=1        MOTOR_SPD=-250       TIMEOUT=30000
 *
 * Grammar (nothing else is accepted, there are no spaces anywhere):
 *
 *     line  := key '=' value [ "\r\n" | "\n" ]
 *     key   := 1 to PARSE_KEY_MAX characters from A-Z, 0-9, '_';
 *              the first character must be a letter A-Z
 *     value := [ '-' ] digit { digit }        (decimal, at least one digit)
 *              in the range PARSE_VALUE_MIN .. PARSE_VALUE_MAX
 *
 * Constraints: no atoi/strtol/sscanf; convert the digits yourself and
 * detect an out-of-range value WITHOUT overflowing an int32_t.
 *
 * Do not change this file.
 */
#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>

#define PARSE_KEY_MAX   10
#define PARSE_VALUE_MIN (-1000000)
#define PARSE_VALUE_MAX 1000000

typedef enum {
    PARSE_OK = 0,
    PARSE_ERR_NULL,         /* line or out is NULL */
    PARSE_ERR_EMPTY,        /* "" or just a line ending */
    PARSE_ERR_NO_EQUALS,    /* no '=' (e.g. "LED1") */
    PARSE_ERR_BAD_KEY,      /* key empty, starts with a non-letter, or has a bad char */
    PARSE_ERR_KEY_TOO_LONG, /* key longer than PARSE_KEY_MAX */
    PARSE_ERR_BAD_VALUE,    /* value empty, "-" alone, non-digit, or junk after it */
    PARSE_ERR_RANGE         /* value outside PARSE_VALUE_MIN..PARSE_VALUE_MAX */
} parse_status_t;

typedef struct {
    char key[PARSE_KEY_MAX + 1]; /* '\0'-terminated */
    int32_t value;
} command_t;

/*
 * Parse one line. On PARSE_OK *out holds the key and value.
 * On any error *out is left completely unchanged.
 *
 * One optional line ending ("\n" or "\r\n") at the very end is ignored.
 * When several things are wrong, the checks happen in this order and the
 * first one that fails decides the result:
 *
 *   1. line or out is NULL                                -> PARSE_ERR_NULL
 *   2. nothing left after removing the line ending        -> PARSE_ERR_EMPTY
 *   3. the key, read from the start up to the first '=':
 *        a char that is not allowed, a first char that is
 *        not A-Z, or '=' as the very first char            -> PARSE_ERR_BAD_KEY
 *        end of line reached without any '='               -> PARSE_ERR_NO_EQUALS
 *        key longer than PARSE_KEY_MAX                     -> PARSE_ERR_KEY_TOO_LONG
 *   4. the value (everything after the '=') is not
 *      [ '-' ] digit { digit }                             -> PARSE_ERR_BAD_VALUE
 *   5. the value is outside PARSE_VALUE_MIN..MAX           -> PARSE_ERR_RANGE
 *
 * Examples: "1LED=5" -> BAD_KEY, "LED5" -> NO_EQUALS, "LED 1" -> BAD_KEY,
 *           "LED=5x" -> BAD_VALUE, "LED=99999999x" -> BAD_VALUE,
 *           "LED=007" -> OK (value 7), "LED=-0" -> OK (value 0).
 */
parse_status_t parse_command(const char *line, command_t *out);

/* Human-readable name of a status, e.g. "PARSE_ERR_RANGE". Unknown -> "UNKNOWN". */
const char *parse_status_str(parse_status_t status);

#endif /* PARSER_H */
