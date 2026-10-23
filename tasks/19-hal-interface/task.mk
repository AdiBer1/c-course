TASK_STAGE      := 6
STUDENT_FILES   := src/led.c src/fake_gpio.c ANSWERS.md
FORBIDDEN_SYMS  := malloc calloc realloc free
EXTRA_CFLAGS    := -Wconversion
HAL_SRCS        := mock_gpio.c
