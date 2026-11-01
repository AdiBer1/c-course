# Task 00 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.
There is no code to write here; these are about *why*.

## Q1. `gcc -c mathx.c` produces `mathx.o`, but that file cannot run. What is missing, and which step adds it?

Answer:
mathx.o הוא התוצאה של קומפילציה של קובץ אחד, ולכן אין בו entry point (main). את זה משלים השלב שנקרא linking: ה-linker מחבר את main.o ו-mathx.o לקובץ הרצה.

## Q2. What would go wrong if `mathx.h` had no include guard and some `.c` file ended up including it twice (directly or through another header)? Would it still go wrong if the header only contained function declarations?

Answer:
בלי include guard, ה-compiler רואה את אותה הגדרה פעמיים ברצף, והקומפילציה נכשלת בשגיאה redefinition of 'struct point'. אם ב-header יש רק הצהרות על פונקציות, זה לא ישתבש: הצהרה על פונקציה רק מודיעה ל-compiler שקיימת פונקציה בשם הזה, עם החתימה הזו, ולכן מותר לחזור עליה כמה פעמים באותו קובץ, כל עוד כל ההצהרות זהות. ה-struct כן נכשל, כי הגדרה של struct יוצרת טיפוס חדש, ומותר להגדיר אותו רק פעם אחת בכל יחידת תרגום.

## Q3. You change only `mathx.c`. Which files does your Makefile rebuild, and why is that better than recompiling everything — especially on a big firmware project?

Answer:
Make משווה זמני שינוי של קבצים. mathx.c חדש יותר מ-mathx.o, ולכן נבנה מחדש רק mathx.o. לאחר מכן קובץ ההרצה עובר linking מחדש, מה-mathx.o החדש ומ-main.o הקיים. main.c לא מקומפל שוב, כי שום דבר שהוא תלוי בו לא השתנה. בפרויקט firmware גדול, עם מאות קבצים ו-drivers, קומפילציה מלאה יכולה לקחת דקות, ובנייה כזו לוקחת שניות. כך מחזור העבודה של עריכה, בנייה, צריבה ובדיקה נשאר מהיר.