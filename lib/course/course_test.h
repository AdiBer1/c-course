/*
 * course_test.h - thin layer on top of Unity used by every task's tests.
 *
 * Test files include this header instead of "unity.h". It replaces Unity's
 * RUN_TEST so a test binary can:
 *   ./test_runner          run every test (normal mode)
 *   ./test_runner -l       list the test names, one per line, without running
 *   ./test_runner -n NAME  run only the test called exactly NAME
 *
 * scripts/score.sh uses -l to know how many tests exist and -n to re-run
 * tests one by one after a crash, so one crashing test does not hide the
 * results of the others.
 */
#ifndef COURSE_TEST_H
#define COURSE_TEST_H

#include "unity.h"

/* Replace Unity's RUN_TEST (after unity.h, so Unity's own declarations stay intact). */
#undef RUN_TEST
#define RUN_TEST(func) course_run_test((func), #func, __LINE__)

/* Call first thing in main(). Reads -l / -n from the command line. */
void course_parse_args(int argc, char **argv);

/* Used by RUN_TEST; do not call directly. */
void course_run_test(UnityTestFunction func, const char *name, int line);

#endif /* COURSE_TEST_H */
