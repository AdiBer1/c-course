#include "command.h"


command_t command_classify(char c) {
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
    (void)current;
    (void)cmd;
    return LED_OFF;
}

const char *command_name(command_t cmd)
{
    (void)cmd;
    return "";
}
