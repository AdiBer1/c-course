# Task 06: Your own string functions

Stage 2, Arrays & pointers.

## What you build

Your own `strlen`, a bounded copy (`strlcpy`-style) and `strcmp`, written with plain loops.

## Why this matters in embedded

Buffer overflows from string handling are the number-one cause of crashes and security holes in connected devices. A firmware engineer must know exactly what a C string is in memory (bytes up to a `'\0'`), why `strcpy` into a fixed buffer is dangerous, and how a bounded copy that always terminates prevents it. Many embedded C libraries also lack the safe variants, so you write them yourself.

## The API

Header `src/my_string.h` (given):

- `size_t my_strlen(const char *s);`
- `size_t my_strlcpy(char *dst, const char *src, size_t dst_size);`
- `int my_strcmp(const char *a, const char *b);`

The documentation in the header is the specification: read it fully before you start. Headers marked "Do not change this file" are not yours to edit.

## Constraints

- No functions from `<string.h>` in your code (checked: `strlen`, `strcpy`, `memcpy`, ... are forbidden symbols).
- Never write outside `dst[0 .. dst_size-1]`.
- Compare characters as `unsigned char`.

Checked automatically for this task:

- Compiler warnings cost points (−5 each).
- Also forbidden in this task: `strlen`, `strnlen`, `strcpy`, `strncpy`, `strlcpy`, `strcmp`, `strncmp`, `memcpy`, `memmove`, `memset`, `strcat`, `strncat` (−15 each).
- cppcheck findings are shown as notes only (no points lost before stage 5).

Files you may change (anything else makes the PR check fail):

- `tasks/06-my-string/src/my_string.c`
- `tasks/06-my-string/ANSWERS.md`

## How to run

Inside `tasks/06-my-string/`:

```bash
make          # build your code and the tests
make test     # build and run the tests (sanitizer reports appear here)
make score    # compute your score exactly like CI does (also: build/score.md)
make clean    # remove build/
```

You need **60 or more** in `make score` before you open the PR. Branch name: `task-06`.

## Understanding questions

Answer the questions in [`ANSWERS.md`](ANSWERS.md), in your own words. An empty `ANSWERS.md` costs 10 points, and the oral defense will start from your answers.
