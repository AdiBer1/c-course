# Task 00 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.
There is no code to write here; these are about *why*.

## Q1. `gcc -c mathx.c` produces `mathx.o`, but that file cannot run. What is missing, and which step adds it?

Answer:
חסרה נקודת הכניסה `main` וה-object files של שאר הקבצים. קובץ `.o` הוא קוד מקומפל אבל לא מקושר — הפניות לפונקציות חיצוניות עדיין לא נפתרות. שלב ה-linking (קישור) הוא שמחבר את כל ה-`.o` files יחד, פותר את הפניות ויוצר קובץ הרצה.

## Q2. What would go wrong if `mathx.h` had no include guard and some `.c` file ended up including it twice (directly or through another header)? Would it still go wrong if the header only contained function declarations?

Answer:
ללא include guard, הגדרות (typedefs, structs, macros) יופיעו פעמיים באותו קובץ — שגיאת קומפיילר "redefinition". אם ה-header מכיל רק הצהרות פונקציות (declarations), הקומפיילר מקבל הצהרה כפולה זהה — זה חוקי ב-C ולא יגרום לשגיאה, אבל עדיין נוהג רע. ה-include guard מונע את הבעיה בכל מקרה.

## Q3. You change only `mathx.c`. Which files does your Makefile rebuild, and why is that better than recompiling everything — especially on a big firmware project?

Answer:
ה-Makefile יבנה מחדש רק את `mathx.o` (כי `mathx.c` השתנה) ואז יקשר מחדש את `hello`. `main.o` לא ישתנה כי לא תלוי ב-`mathx.c`. בפרויקט firmware גדול עם מאות קבצים, קומפילציה מחדש של הכל יכולה לקחת דקות. בנייה מצטברת (incremental build) מקמפלת רק מה שהשתנה — חוסכת זמן ומאפשרת פיתוח מהיר.
