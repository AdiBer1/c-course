# Task 05: Output parameters and error codes

Stage 2, Arrays & pointers.

## What you build

Functions that return several results through pointers and report success or failure with a status code.

## Why this matters in embedded

Almost every embedded driver API has this shape: `status_t adc_read(uint8_t ch, uint16_t *value_out)`. The return value says whether it worked (bus error? timeout?) and the result goes through a pointer. Callers that ignore the status, or functions that write garbage into outputs before failing, cause the most common class of bugs in production firmware.

## The API

Header `src/outparams.h` (given):

- `void swap_i32(int32_t *a, int32_t *b);`
- `op_status_t min_max_i16(const int16_t *values, size_t len, int16_t *min_out, int16_t *max_out);`
- `op_status_t divmod_u32(uint32_t dividend, uint32_t divisor, uint32_t *quotient, uint32_t *remainder);`
- `op_status_t sort3_i32(int32_t *a, int32_t *b, int32_t *c);`

The documentation in the header is the specification: read it fully before you start. Headers marked "Do not change this file" are not yours to edit.

## Constraints

- Check every pointer for `NULL` before use.
- On any error, never write to an output parameter.

Checked automatically for this task:

- Compiler warnings cost points (−5 each).
- cppcheck findings are shown as notes only (no points lost before stage 5).

Files you may change (anything else makes the PR check fail):

- `tasks/05-out-params/src/outparams.c`
- `tasks/05-out-params/ANSWERS.md`

## How to run

Inside `tasks/05-out-params/`:

```bash
make          # build your code and the tests
make test     # build and run the tests (sanitizer reports appear here)
make score    # compute your score exactly like CI does (also: build/score.md)
make clean    # remove build/
```

You need **60 or more** in `make score` before you open the PR. Branch name: `task-05`.

## Understanding questions

Answer the questions in [`ANSWERS.md`](ANSWERS.md), in your own words. An empty `ANSWERS.md` costs 10 points, and the oral defense will start from your answers.
