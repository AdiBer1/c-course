# common.mk - shared build logic for every task.
#
# Each task's Makefile is just:
#     include task.mk
#     include ../../common.mk
#
# Targets:
#   make         build everything (your code + the tests), do not run
#   make test    build and run the tests
#   make score   compute your score exactly like CI does
#   make memcheck   run the tests under Valgrind (optional, needs valgrind)
#   make format     show clang-format differences (optional, changes nothing)
#   make clean   delete the build/ folder
#
# Variables a task.mk may set:
#   TASK_STAGE      stage number 0..6 (switches checks on, see README)
#   STUDENT_FILES   the only files a pull request may change
#   FORBIDDEN_SYMS  extra functions that must not be used
#   EXTRA_CFLAGS    extra compiler flags for the student's code
#   HAL_SRCS        files from /hal to link in (e.g. mock_gpio.c)
#   TEST_EXCLUDE    src/*.c files that are not linked into the tests

ROOT  := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
TASK  := $(notdir $(CURDIR))
BUILD := build

ifeq ($(origin CC),default)
CC := gcc
endif

TASK_STAGE     ?= 0
STUDENT_FILES  ?=
FORBIDDEN_SYMS ?=
EXTRA_CFLAGS   ?=
HAL_SRCS       ?=
TEST_EXCLUDE   ?=

stage_at_least = $(shell [ $(TASK_STAGE) -ge $(1) ] && echo yes)

# Checks that switch on by stage.
STAGE_FORBIDDEN := $(if $(call stage_at_least,3),malloc calloc realloc free)
ALL_FORBIDDEN   := $(sort $(STAGE_FORBIDDEN) $(FORBIDDEN_SYMS))
STAGE_CFLAGS    := $(if $(call stage_at_least,4),-Wconversion)
CPPCHECK_COUNTS := $(if $(call stage_at_least,5),1,0)

# Compiler flags. Warnings never stop the build; they cost points instead.
BASE_CFLAGS := -std=c11 -Wall -Wextra -pedantic -Wshadow -g
TASK_CFLAGS := $(STAGE_CFLAGS) $(filter-out $(STAGE_CFLAGS),$(EXTRA_CFLAGS))
SAN_FLAGS   := -fsanitize=address,undefined -fsanitize-recover=address,undefined -fno-omit-frame-pointer
INCLUDES    := -Isrc -I$(ROOT)/hal -I$(ROOT)/lib/unity -I$(ROOT)/lib/course
DEPFLAGS    := -MMD -MP

STUDENT_CFLAGS := $(BASE_CFLAGS) $(TASK_CFLAGS) $(SAN_FLAGS)
SUPPORT_CFLAGS := $(BASE_CFLAGS) $(SAN_FLAGS)

# Sanitizers keep running after an error so every report can be counted.
# (Leak detection exists on Linux/WSL only.)
DETECT_LEAKS := $(if $(filter Linux,$(shell uname -s)),1,0)
SAN_ENV := ASAN_OPTIONS=halt_on_error=0:abort_on_error=0:detect_leaks=$(DETECT_LEAKS):detect_stack_use_after_return=1:print_summary=1 \
           UBSAN_OPTIONS=halt_on_error=0:print_stacktrace=1
TIMEOUT := $(shell command -v timeout >/dev/null 2>&1 && echo timeout 5s)

SRCS      := $(filter-out $(TEST_EXCLUDE),$(wildcard src/*.c))
TEST_SRCS := $(wildcard tests/*.c)

OBJS      := $(SRCS:%.c=$(BUILD)/%.o)
TEST_OBJS := $(TEST_SRCS:%.c=$(BUILD)/%.o)
HAL_OBJS  := $(HAL_SRCS:%.c=$(BUILD)/hal/%.o)
LIB_OBJS  := $(BUILD)/lib/unity.o $(BUILD)/lib/course_test.o
TEST_BIN  := $(BUILD)/test_runner

.DEFAULT_GOAL := all
.PHONY: all build test score memcheck format clean config

all build: $(TEST_BIN)

$(TEST_BIN): $(OBJS) $(TEST_OBJS) $(HAL_OBJS) $(LIB_OBJS)
	$(CC) $(SAN_FLAGS) $^ -o $@

# Your code: full warnings for the stage.
$(BUILD)/src/%.o: src/%.c task.mk
	@mkdir -p $(@D)
	$(CC) $(STUDENT_CFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@

# Tests and the mock hardware layer.
$(BUILD)/tests/%.o: tests/%.c
	@mkdir -p $(@D)
	$(CC) $(SUPPORT_CFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD)/hal/%.o: $(ROOT)/hal/%.c
	@mkdir -p $(@D)
	$(CC) $(SUPPORT_CFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@

# Test framework: compiled quietly, it is not your code.
$(BUILD)/lib/unity.o: $(ROOT)/lib/unity/unity.c
	@mkdir -p $(@D)
	@$(CC) -std=c11 -g -w $(SAN_FLAGS) $(INCLUDES) -c $< -o $@

$(BUILD)/lib/course_test.o: $(ROOT)/lib/course/course_test.c
	@mkdir -p $(@D)
	@$(CC) -std=c11 -g -w $(SAN_FLAGS) $(INCLUDES) -c $< -o $@

test: $(TEST_BIN)
	@$(SAN_ENV) $(TIMEOUT) ./$(TEST_BIN); status=$$?; \
	if [ $$status -eq 124 ]; then echo "*** Tests stopped after 5 s: infinite loop or too slow"; fi; \
	exit $$status

score:
	@bash $(ROOT)/scripts/score.sh .

memcheck:
	@bash $(ROOT)/scripts/memcheck.sh .

format:
	@bash $(ROOT)/scripts/format-check.sh .

clean:
	rm -rf $(BUILD)

# Machine-readable task settings, used by the scripts in /scripts.
config:
	@echo "TASK=$(TASK)"
	@echo "TASK_STAGE=$(TASK_STAGE)"
	@echo "STUDENT_FILES=$(STUDENT_FILES)"
	@echo "FORBIDDEN=$(ALL_FORBIDDEN)"
	@echo "CPPCHECK_COUNTS=$(CPPCHECK_COUNTS)"
	@echo "STUDENT_CFLAGS=$(strip $(STUDENT_CFLAGS))"
	@echo "INCLUDES=$(INCLUDES)"

-include $(wildcard $(BUILD)/*/*.d)
