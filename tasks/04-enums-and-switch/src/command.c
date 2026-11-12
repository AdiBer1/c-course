#include "command.h"

command_t command_classify(char c)
{
    switch (c) {
    case ' ':
    case '\r':
    case '\n':
    case '\t':
        return CMD_NONE;
    case '1':
        return CMD_LED_ON;
    case '0':
        return CMD_LED_OFF;
    case 't':
    case 'T':
        return CMD_LED_TOGGLE;
    case 's':
    case 'S':
        return CMD_STATUS;
    case 'r':
    case 'R':
        return CMD_RESET;
    default:
        return CMD_UNKNOWN;
    }
}

led_state_t led_next_state(led_state_t current, command_t cmd)
{
    /* An invalid current state (corrupted value) is treated as LED_OFF. */
    if (current != LED_ON) {
        current = LED_OFF;
    }

    switch (cmd) {
    case CMD_LED_ON:
        return LED_ON;
    case CMD_LED_OFF:
    case CMD_RESET:
        return LED_OFF;
    case CMD_LED_TOGGLE:
        return (current == LED_ON) ? LED_OFF : LED_ON;
    case CMD_NONE:
    case CMD_STATUS:
    case CMD_UNKNOWN:
    case CMD_COUNT:
        return current;
    default:
        /* Out-of-range command (e.g. 42): do nothing, keep the state. */
        return current;
    }
}

const char *command_name(command_t cmd)
{
    /*
     * Returning string literals is safe: they are stored in read-only
     * memory for the whole life of the program, not on the stack.
     */
    switch (cmd) {
    case CMD_NONE:
        return "NONE";
    case CMD_LED_ON:
        return "LED_ON";
    case CMD_LED_OFF:
        return "LED_OFF";
    case CMD_LED_TOGGLE:
        return "LED_TOGGLE";
    case CMD_STATUS:
        return "STATUS";
    case CMD_RESET:
        return "RESET";
    case CMD_UNKNOWN:
        return "UNKNOWN";
    case CMD_COUNT:
        return "INVALID";
    default:
        /* Any corrupted / out-of-range value. */
        return "INVALID";
    }
}