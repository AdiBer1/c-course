# Task 01 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `make score` already gives you a score on your laptop. Why does CI build and test your code again on a fresh machine instead of trusting your local result?

Answer: 
כי הסביבה שלי שונה מבחינת ספריות שמותקנות, גרסאות קומפיילר וקבצים מקומיים וחשוב לוודא שהקוד עובר בדיקה בסביבה נקיה.

## Q2. Your PR may only change `src/max3.c` and `ANSWERS.md`. Why is it important that you cannot change the tests (think about what a test is *for*)?

Answer: 
כי אם הייתה לי אפשרות לשנות את הטסט הייתי יכולה לשנות אותו כך שייתן לי ציון 100 עבור הקוד שלי גם אם הוא שגוי.

## Q3. In `max3`, the parameters `a`, `b`, `c` are copies of the caller's values. What does that mean if your function changes `a`? Does the caller notice?

Answer: 
לא, משתנה שמועבר כהעתק מוכר רק בתוך הscope של הפונקציה ושינוי בתוך הפונקציה לא יהיה מוכר מחוצה לה.

