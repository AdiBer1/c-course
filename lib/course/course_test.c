#include "course_test.h"

#include <stdio.h>
#include <string.h>

static int list_only;
static const char *only_name;

void course_parse_args(int argc, char **argv)
{
    /* Line-buffer stdout so results already printed survive a later crash. */
    setvbuf(stdout, NULL, _IOLBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0) {
            list_only = 1;
        } else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            only_name = argv[++i];
        }
    }
}

void course_run_test(UnityTestFunction func, const char *name, int line)
{
    if (list_only) {
        printf("TEST:%s\n", name);
        return;
    }
    if (only_name != NULL && strcmp(only_name, name) != 0) {
        return;
    }
    UnityDefaultTestRun(func, name, line);
}
