# Task 06 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `char buf[4]; strcpy(buf, "LED=1");` compiles without error. What exactly happens in memory, and why can it crash much later in a completely different function?

Answer:
המחרוזת `"LED=1"` היא 6 בתים (5 תווים ועוד `'\0'`), אבל `buf` הוא רק 4 בתים. `strcpy` לא יודעת מה גודל היעד ופשוט מעתיקה עד ה-`'\0'`, ולכן שני הבתים האחרונים (`'1'` ו-`'\0'`) נכתבים מעבר לסוף המערך, על מה שנמצא אחריו במחסנית: משתנה מקומי אחר, saved registers או כתובת החזרה. הקומפיילר לא בודק את זה. הנזק מתגלה רק כשמישהו משתמש בזיכרון שנדרס: משתנה אחר מקבל ערך מוזר, או שהפונקציה חוזרת לכתובת שגויה. זה יכול לקרות הרבה אחרי ההעתקה ובפונקציה אחרת לגמרי, ולכן קשה מאוד למצוא את הבאג.

## Q2. Why does `my_strlcpy` return the length of `src` rather than the number of bytes copied? Show how a caller uses that value.

Answer:
את מספר הבתים שהועתקו הקורא יכול לחשב בעצמו (לכל היותר `dst_size - 1`). מה שהוא לא יודע זה האם המחרוזת **נחתכה**. כשמחזירים את האורך המלא של `src`, הקורא משווה אותו לגודל הבאפר:
```c
char buf[8];
if (my_strlcpy(buf, name, sizeof buf) >= sizeof buf) {
    /* name was truncated: report an error, or use a bigger buffer */
}
```
הערך גם אומר בדיוק כמה מקום היה צריך (`len + 1`).

## Q3. Why must `my_strcmp` compare characters as `unsigned char`? Give an example where using plain `char` gives the wrong order.

Answer:
בהרבה מערכות (למשל x86) `char` הוא signed, כך שתווים מ-128 עד 255 נקראים כמספרים שליליים. ההגדרה של strcmp היא השוואה לפי ערכי הבתים כ-unsigned. דוגמה: `"\xE9"` (é ב-Latin-1, ערך 233) מול `"A"` (65). כ-unsigned מתקיים 233 > 65 ולכן é אחרי A, וזה נכון. כ-signed char ‏`0xE9` הוא ‎-23‎, ואז ‎-23 < 65‎ ו-é ממוינת **לפני** A, כלומר הסדר שגוי. גם התוצאה הייתה תלויה בקומפיילר ובמעבד.

## Q4. `sizeof buf` and `my_strlen(buf)` for `char buf[16] = "hi";` — what are the two values and why are they different?

Answer:
`sizeof buf` הוא 16: גודל המערך בזיכרון בבתים, שהקומפיילר יודע בזמן קומפילציה. `my_strlen(buf)` הוא 2: מספר התווים עד ה-`'\0'` הראשון, שמחושב בזמן ריצה. השאר (ה-`'\0'` ושאר הבתים, שאותחלו לאפס) הם קיבולת שלא בשימוש. אחד הוא כמה **מקום** יש, והשני הוא כמה **תוכן** יש.
