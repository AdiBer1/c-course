# Task 07 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. How does your parser detect that a value is out of range without ever overflowing `int32_t`? Why is "just use `int64_t`" not a complete answer?

Answer:
הפרסר צובר את הערך המוחלט (magnitude) ספרה אחרי ספרה, ו**לפני** כל צעד בודק אם `magnitude * 10 + digit` יעבור את הגבול. הבדיקה כתובה כ-`magnitude > (limit - digit) / 10`, כך שאף פעם לא מחשבים מספר גדול מהגבול. ברגע שעוברים את הגבול מפסיקים לצבור, אבל ממשיכים לסרוק עד הסוף, כדי ש-`"LED=99999999x"` יחזיר BAD_VALUE ולא RANGE. ‏`int64_t` לא פותר את הבעיה: הקלט יכול להכיל כמה ספרות שרוצים (למשל 30 ספרות), ואז גם `int64_t` גולש. בנוסף, על מיקרו-בקר של 32 ביט חישובי 64 ביט איטיים וגדולים יותר.

## Q2. Why must `*out` stay unchanged on every error? Describe what could go wrong on a device if a half-parsed command were left in `out`.

Answer:
הקורא יכול להחזיק ב-`out` את הפקודה התקינה האחרונה, או לא לבדוק את הסטטוס כמו שצריך. אם הפרסר כבר העתיק את המפתח ורק אחר כך גילה שהערך לא חוקי, נשאר ב-`out` שילוב של מפתח חדש עם ערך ישן. למשל, אחרי `MOTOR_SPD=1` ואז `HEATER=9x`, נקבל `HEATER` עם ערך 1, והמכשיר עלול להדליק את המחמם על סמך פקודה שבכלל נדחתה. לכן כותבים ל-`out` רק בסוף, אחרי שכל הבדיקות עברו.

## Q3. The input line comes from a serial port, typed by a human or sent by another device. Why should a parser assume the input can be anything, including very long or malicious lines?

Answer:
הפרסר לא שולט במה שמגיע מבחוץ: אדם יכול לטעות בהקלדה, רעש על הקו משבש בתים, מכשיר אחר יכול להיות תקול, ותוקף יכול לשלוח בכוונה שורה ארוכה או מספר ענק כדי לגרום לגלישת באפר או ל-overflow. אם הפרסר מניח קלט "סביר", קלט כזה יכול לדרוס זיכרון, להפיל את המכשיר או לבצע פקודה שגויה. לכן כל אורך וכל תו נבדקים, ושום דבר לא נכתב לפני שהשורה כולה נמצאה חוקית.
