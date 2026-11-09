# Task 01 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `make score` already gives you a score on your laptop. Why does CI build and test your code again on a fresh machine instead of trusting your local result?

Answer:
כי הסביבה המקומית שלי עשויה להיות שונה — ספריות אחרות, הגדרות קומפיילר שונות, קבצים שלא הועלו ל-Git. CI בונה על מכונה נקייה ומוגדרת אחידה, כך שהתוצאה מהימנה ועקבית לכולם.

## Q2. Your PR may only change `src/max3.c` and `ANSWERS.md`. Why is it important that you cannot change the tests (think about what a test is *for*)?

Answer:
הבדיקות הן הקריטריון האובייקטיבי לנכונות הקוד. אם אפשר לשנות אותן, אפשר לגרום לכל קוד לעבור — כולל קוד שגוי. הבדיקות חייבות להישאר קבועות כדי שיהיו משמעותיות.

## Q3. In `max3`, the parameters `a`, `b`, `c` are copies of the caller's values. What does that mean if your function changes `a`? Does the caller notice?

Answer:
הפרמטרים מועברים by value — הפונקציה מקבלת עותק. אם הפונקציה משנה את `a`, היא משנה רק את העותק המקומי שלה. הקורא לא מרגיש שום שינוי במשתנה המקורי שלו.

