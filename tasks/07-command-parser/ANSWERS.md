# Task 07 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. How does your parser detect that a value is out of range without ever overflowing `int32_t`? Why is "just use `int64_t`" not a complete answer?

Answer:
לפני כל כפל בודקים אם `val > (MAX - d) / 10` — אם כן, מסמנים range error בלי לבצע את הכפל. `int64_t` לא מספיק כי קלט ארוך מאוד (20+ ספרות) יכול להציף גם `int64_t`, ובמיקרוקונטרולרים רבים `int64_t` איטי או איטי.

## Q2. Why must `*out` stay unchanged on every error? Describe what could go wrong on a device if a half-parsed command were left in `out`.

Answer:
אם `*out` ישתנה בשגיאה, הקוד הקורא עלול לבדוק רק את קוד השגיאה ועדיין להשתמש בערך חצאי מפורס. לדוגמה: מנוע שמקבל פקודה שגויה עם מפתח נכון אבל ערך שגוי עלול לפעול בצורה לא צפויה.

## Q3. The input line comes from a serial port, typed by a human or sent by another device. Why should a parser assume the input can be anything, including very long or malicious lines?

Answer:
קלט מסדרתי יכול להיות שגוי, פגום או זדוני. פרסר שלא מגביל אורך קלט עלול להיות קורבן ל-buffer overflow. פרסר שלא בודק תווים לא חוקיים עלול להפעיל פקודות לא צפויות במערכת.

