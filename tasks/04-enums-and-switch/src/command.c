#include "command.h"

/* TODO: implement every function with a switch statement. */

command_t command_classify(char c)
{
    (void)c;
    return CMD_NONE;
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
