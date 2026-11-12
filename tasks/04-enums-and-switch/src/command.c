#include "command.h"

/* TODO: implement every function with a switch statement. */

command_t command_classify(char c)
{
    switch (c)
    {
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
    /* Handle invalid LED states. */
    switch (current)
    {
        case LED_OFF:
        case LED_ON:
            break;

        default:
            current = LED_OFF;
            break;
    }

    switch (cmd)
    {
        case CMD_LED_ON:
            return LED_ON;

        case CMD_LED_OFF:
        case CMD_RESET:
            return LED_OFF;

        case CMD_LED_TOGGLE:
            switch (current)
            {
                case LED_OFF:
                    return LED_ON;

                case LED_ON:
                    return LED_OFF;

                default:
                    return LED_OFF;
            }

        default:
            return current;
    }
}

const char *command_name(command_t cmd)
{
    switch (cmd)
    {
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

        default:
            return "INVALID";
    }
}