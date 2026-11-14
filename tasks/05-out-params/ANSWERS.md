# Task 05 — Understanding questions

## Q1. Why can't swap_i32 without pointers swap the caller's variables?

Answer:
Because C passes copies of the values. Changing the copies does not change the original variables. Pointers let us change the originals.

## Q2. Why does min_max_i16 use const for values but not for the output pointers?

Answer:
The array is only for reading, so we use const. The output pointers need to store the results. Changing values[0] causes a compiler error.

## Q3. How can changing outputs on error cause a bug in firmware?

Answer:
A sensor might fail to read the temperature but still change the output. The system could use a wrong value and control a device incorrectly.

## Q4. Why return a status code instead of using -1 for errors?

Answer:
Because -1 can also be a valid result. A status code clearly shows if the operation succeeded or failed.
