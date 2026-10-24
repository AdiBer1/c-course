#define _POSIX_C_SOURCE 200809L /* for popen(), access() */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "course_test.h"
#include "mathx.h"

void setUp(void) {}
void tearDown(void) {}

/* ---------------------------------------------------------------- mathx */

static void test_square_of_positive_number(void)
{
    TEST_ASSERT_EQUAL_INT(49, square(7));
}

static void test_square_of_negative_number(void)
{
    TEST_ASSERT_EQUAL_INT(9, square(-3));
}

static void test_square_of_zero(void)
{
    TEST_ASSERT_EQUAL_INT(0, square(0));
}

static void test_clamp_value_inside_range_is_unchanged(void)
{
    TEST_ASSERT_EQUAL_INT(15, clamp(15, 10, 20));
}

static void test_clamp_value_below_range_returns_lo(void)
{
    TEST_ASSERT_EQUAL_INT(10, clamp(3, 10, 20));
}

static void test_clamp_value_above_range_returns_hi(void)
{
    TEST_ASSERT_EQUAL_INT(100, clamp(150, 0, 100));
}

static void test_clamp_value_on_the_edges_is_unchanged(void)
{
    TEST_ASSERT_EQUAL_INT(10, clamp(10, 10, 20));
    TEST_ASSERT_EQUAL_INT(20, clamp(20, 10, 20));
}

static void test_clamp_with_negative_range(void)
{
    TEST_ASSERT_EQUAL_INT(-20, clamp(-50, -20, -10));
    TEST_ASSERT_EQUAL_INT(-10, clamp(0, -20, -10));
}

/* ---------------------------------------------------------------- your Makefile */

/* Run `make <args>` inside src/ and return make's exit code. */
static int make_in_src(const char *args)
{
    char cmd[256];
    snprintf(cmd, sizeof cmd,
             "env -u MAKEFLAGS -u MAKELEVEL -u MFLAGS make -s -C src %s >/dev/null 2>&1", args);
    int status = system(cmd);
    return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
}

static void remove_outputs(void)
{
    (void)system("rm -f src/hello src/*.o");
}

static int exists(const char *path)
{
    return access(path, F_OK) == 0;
}

static void test_makefile_builds_hello(void)
{
    remove_outputs();
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, make_in_src(""), "`make` inside src/ failed");
    TEST_ASSERT_TRUE_MESSAGE(exists("src/hello"), "src/hello was not created");
}

static void test_makefile_compiles_each_file_to_an_object(void)
{
    remove_outputs();
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, make_in_src(""), "`make` inside src/ failed");
    TEST_ASSERT_TRUE_MESSAGE(exists("src/main.o"), "src/main.o was not created");
    TEST_ASSERT_TRUE_MESSAGE(exists("src/mathx.o"), "src/mathx.o was not created");
}

static void test_hello_prints_expected_output(void)
{
    remove_outputs();
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, make_in_src(""), "`make` inside src/ failed");

    char out[512] = {0};
    FILE *p = popen("./src/hello", "r");
    TEST_ASSERT_NOT_NULL(p);
    size_t n = fread(out, 1, sizeof out - 1, p);
    out[n] = '\0';
    pclose(p);

    TEST_ASSERT_NOT_NULL_MESSAGE(strstr(out, "Hello from C!"), out);
    TEST_ASSERT_NOT_NULL_MESSAGE(strstr(out, "square(7) = 49"), out);
    TEST_ASSERT_NOT_NULL_MESSAGE(strstr(out, "clamp(150, 0, 100) = 100"), out);
}

static void test_makefile_rebuilds_when_header_changes(void)
{
    remove_outputs();
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, make_in_src(""), "`make` inside src/ failed");

    /* Make everything old, then "edit" only the header. */
    (void)system("touch -t 200001010000 src/*.c src/*.h src/*.o src/hello src/Makefile");
    (void)system("touch src/mathx.h");

    /* make -q exits 1 when something must be rebuilt, 0 when all is up to date. */
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, make_in_src("-q"),
                                  "after mathx.h changed, make thinks everything is up to date: "
                                  "the .o files must depend on mathx.h");
}

static void test_makefile_clean_removes_outputs(void)
{
    remove_outputs();
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, make_in_src(""), "`make` inside src/ failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, make_in_src("clean"), "`make clean` failed");
    TEST_ASSERT_FALSE_MESSAGE(exists("src/hello"), "make clean left src/hello");
    TEST_ASSERT_FALSE_MESSAGE(exists("src/main.o"), "make clean left src/main.o");
    TEST_ASSERT_FALSE_MESSAGE(exists("src/mathx.o"), "make clean left src/mathx.o");
}

int main(int argc, char **argv)
{
    course_parse_args(argc, argv);
    UNITY_BEGIN();
    RUN_TEST(test_square_of_positive_number);
    RUN_TEST(test_square_of_negative_number);
    RUN_TEST(test_square_of_zero);
    RUN_TEST(test_clamp_value_inside_range_is_unchanged);
    RUN_TEST(test_clamp_value_below_range_returns_lo);
    RUN_TEST(test_clamp_value_above_range_returns_hi);
    RUN_TEST(test_clamp_value_on_the_edges_is_unchanged);
    RUN_TEST(test_clamp_with_negative_range);
    RUN_TEST(test_makefile_builds_hello);
    RUN_TEST(test_makefile_compiles_each_file_to_an_object);
    RUN_TEST(test_hello_prints_expected_output);
    RUN_TEST(test_makefile_rebuilds_when_header_changes);
    RUN_TEST(test_makefile_clean_removes_outputs);
    return UNITY_END();
}
