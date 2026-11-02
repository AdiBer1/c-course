# Task 01 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `make score` already gives you a score on your laptop. Why does CI build and test your code again on a fresh machine instead of trusting your local result?

Answer:
כי יכול להיות שקיימים אצלי קבצים שלא נדחפו: במחשב שלי הכול קיים, אבל ב-repo יהיו קבצים חסרים.
וגם אי אפשר לסמוך על בדיקות שרצו במחשב שלי, כי יכולנו למחוק אותן או לשנות אותן.
ועוד משהו אם אני התקנתי סביבה שונה: גרסת compiler אחרת, הקוד יכול לא לעבוד אצל אחרים.

## Q2. Your PR may only change `src/max3.c` and `ANSWERS.md`. Why is it important that you cannot change the tests (think about what a test is *for*)?

Answer:
כי אנחנו יכולות פשוט למחוק או לשנות את הבדיקה כדי שתעבור בהצלחה במקום לתקן את הטעות ובנוסף יכול להיות שמבחינתנו הבדיקה עברה תקין אבל מבחינת הכותב יש לו בדיקות נוספות ודברים אחרים שהוא מאיין כתקין.


## Q3. In `max3`, the parameters `a`, `b`, `c` are copies of the caller's values. What does that mean if your function changes `a`? Does the caller notice?

Answer:
לא, a הוא עותק שיושב בתא זיכרון משלו, נפרד מ-x. ולכן כשהפונקציה משנה את a, היא משנה רק את העותק, והמקור אצל הקורא נשאר כמו שהיה.

