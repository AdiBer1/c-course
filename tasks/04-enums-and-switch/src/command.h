/*
 * command.h - single-character commands from a serial console.
 *
 * Small devices often accept one-letter commands over a UART: you type a
 * character in a terminal and the firmware reacts. This module turns the
 * received character into an enum, and the enum into the next LED state.
 *
 * Do not change this file.
 */
#ifndef COMMAND_H
#define COMMAND_H

typedef enum {
    CMD_NONE = 0,   /* whitespace: ' ', '\r', '\n', '\t' -> ignore quietly */
    CMD_LED_ON,     /* '1'                                                  */
    CMD_LED_OFF,    /* '0'                                                  */
    CMD_LED_TOGGLE, /* 't' or 'T'                                           */
    CMD_STATUS,     /* 's' or 'S'                                           */
    CMD_RESET,      /* 'r' or 'R'                                           */
    CMD_UNKNOWN,    /* any other character -> reply with an error           */
    CMD_COUNT       /* number of commands; not a real command               */
} command_t;

typedef enum {
    LED_OFF = 0,
    LED_ON
} led_state_t;

/* Classify one received character. */
command_t command_classify(char c);

/*
 * The LED state after executing `cmd` in state `current`:
 *   CMD_LED_ON -> LED_ON, CMD_LED_OFF -> LED_OFF, CMD_LED_TOGGLE -> the other
 *   state, CMD_RESET -> LED_OFF; every other command leaves it unchanged.
 * An invalid `current` (not LED_OFF or LED_ON) is treated as LED_OFF.
 */
led_state_t led_next_state(led_state_t current, command_t cmd);

/*
 * A printable name for the command, exactly the enum name without "CMD_":
 * "NONE", "LED_ON", "LED_OFF", "LED_TOGGLE", "STATUS", "RESET", "UNKNOWN".
 * Any value outside CMD_NONE..CMD_UNKNOWN (including CMD_COUNT) -> "INVALID".
 * The returned string is a string literal: never modify or free it.
 */
const char *command_name(command_t cmd);

#endif /* COMMAND_H */
