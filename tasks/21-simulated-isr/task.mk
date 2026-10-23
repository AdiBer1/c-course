TASK_STAGE      := 6
STUDENT_FILES   := src/uart_rx.c ANSWERS.md
FORBIDDEN_SYMS  := malloc calloc realloc free
EXTRA_CFLAGS    := -Wconversion
HAL_SRCS        := sim_uart.c
