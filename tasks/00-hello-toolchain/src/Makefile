# Your own Makefile for the "hello" program. See the task README.
#
# Requirements:
#   - `make` (inside src/) builds the program ./hello
#   - each .c file is first compiled to its own .o file (main.o, mathx.o),
#     then the .o files are linked into hello
#   - if mathx.h changes, the files that include it are rebuilt
#   - `make clean` deletes hello and all .o files
#
# Remember: the command line of a rule starts with a TAB, not spaces.

hello:
# 	//@echo "TODO: write the rules that build hello (see the task README)" && false
# 	@gcc -c main.c -o main.o
# 	@gcc -c mathx.c -o mathx.o
# 	@gcc main.o mathx.o -o hello

	# הגדרת המשתנים: הקומפילטור והדגלים (מונע אזהרות שמורידות נקודות)
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# המטרה הראשית: יצירת קובץ ההרצה hello מתוך קובצי ה-o.
hello: main.o mathx.o
	$(CC) $(CFLAGS) main.o mathx.o -o hello

# קומפילציה נפרדת של main.c לקובץ main.o
# שים לב: הקובץ תלוי גם ב-mathx.h
main.o: main.c mathx.h
	$(CC) $(CFLAGS) -c main.c -o main.o

# קומפילציה נפרדת של mathx.c לקובץ mathx.o
mathx.o: mathx.c mathx.h
	$(CC) $(CFLAGS) -c mathx.c -o mathx.o

# מטרת ניקוי: מוחקת את תוצרי הבנייה
clean:
	rm -f hello main.o mathx.o

.PHONY: clean
