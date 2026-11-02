# Task 00 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.
There is no code to write here; these are about *why*.

## Q1. `gcc -c mathx.c` produces `mathx.o`, but that file cannot run. What is missing, and which step adds it?

Answer:

<div dir="rtl">

ב-`mathx.o` חסרה הפונקציה `main` שמפעילה את התוכנית. צריך לעשות link כדי לקשר את `main.o` ל-`mathx.o`.

</div>


## Q2. What would go wrong if `mathx.h` had no include guard and some `.c` file ended up including it twice (directly or through another header)? Would it still go wrong if the header only contained function declarations?

Answer:
<div dir="rtl">

בכל פעם שכותבים `#include "mathx.h"` בקובץ `.c`, התוכן של ה-header מועתק לתוך הקובץ. אם הוא נכלל פעמיים, הקוד מגיע פעמיים, ואם יש בו הגדרות הן יופיעו פעמיים ונקבל שגיאת redefinition.
אם יש בקובץ רק הצהרות, לא תהיה שגיאה, כי לא אכפת לקומפיילר שיהיו הצהרות שחוזרות על עצמן.
אבל עדיין חשוב להוסיף הגנה (include guard), כי קובץ שהיו בו היום רק הצהרות יכול מחר להכיל גם הגדרות.

</div>
## Q3. You change only `mathx.c`. Which files does your Makefile rebuild, and why is that better than recompiling everything — especially on a big firmware project?

Answer:
<div dir="rtl">

כל קבצי הO נבנים מקבצי C. אם משנים את C אז צריך לבנות מחדש את קובץ הO, אבל חבל לבנות את כל הקבצים של כלללל הפרויקט שוב, הרעיון של Makefile  שהוא בונה רק את הקבצים שתלויים בקובץ שהשתנה שוב. לכן אך ורק הקובץ `mathx.o` יבנה שוב והקבצים שתלויים בו. בפרויקטים גדולים זה יותר משמעותי מהפרויקט הזה, כי לבנות את כל הקבצים מחדש יכול לקחת באמת הרבה זמן

</div>

