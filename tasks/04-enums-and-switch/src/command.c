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
  if (current != LED_OFF && current != LED_ON) {
        current = LED_OFF;
    }
    switch (cmd) {
        case CMD_LED_ON:
            return LED_ON;
        case CMD_LED_OFF:
            return LED_OFF;
        case CMD_LED_TOGGLE:
            return (current == LED_OFF) ? LED_ON : LED_OFF;
        case CMD_RESET:
            return LED_OFF;
        default:
            return current;
    }
}

const char *command_name(command_t cmd)
{
    (void)cmd;
    return "";
}
