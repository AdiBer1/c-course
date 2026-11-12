# Task 03 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `9 / 5 * c10` and `c10 * 9 / 5` look the same in math. Why do they give different results in C, and which one is right?

Answer:
הנכון הוא קודם להכפיל ואז לחלק, כי חילוק בשפת C מעגל את התוצאה כלפי מטה ואם ממשיכים להכפיל אח"כ מאבדים את הדיוק 
## Q2. Explain how you round to the nearest integer for a negative result such as -41.8. Why does the trick "add half the divisor" alone fail for negative numbers?

Answer:
כייון שבמספרים שליליים עיגול כלפי מטה הוא לא כלפי האפס אלא להפך לכן צריך להחסיר חצי מכנה ולא להוסיף.
## Q3. On an 8-bit AVR, `int` is 16 bits. What would happen in `adc_to_mv` if the multiplication `raw * 3300` were done in `int` there? How did you make your code safe on such a chip?

Answer:
המכפלה הייתה יוצרת גלישה וכדי למנוע זאת המרנו את משתנה הקלט לuint32_t מראש לפי ההכפלה וזה גורם שהחישה יתבצע במרחב בטוח של 32 סיביות.
## Q4. Why would firmware on a small microcontroller avoid `float` for this kind of conversion?

Answer:
כיוון שזה מצריך ספריות עיבוד כבדות שגוזלות משאבי זמן ומקום.
