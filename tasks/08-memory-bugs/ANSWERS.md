# Task 08 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Before your fix, `log_add` wrote `values[8]` when the log was full, and most of the time nothing visibly broke. What memory did it overwrite, and why is "it seemed to work" so dangerous?

Answer:
כתב מעבר לגבול המערך לתוך זיכרון שייך למשתנים אחרים (כמו `count` או `label`). "נראה שעובד" מסוכן כי הבאג מסתתר עד שמשהו אחר משתמש באותו זיכרון — ואז הקריסה קורית במקום אחר לגמרי, קשה לאיתור.

## Q2. Explain the bug in `log_label` using the words stack, lifetime and dangling pointer. Why did making the buffer `static` fix it?

Answer:
`buf` הוגדר על ה-stack של `log_label`. כשהפונקציה חזרה, ה-lifetime של `buf` הסתיים והזיכרון שוחרר. `label` הפך ל-dangling pointer שמצביע לזיכרון לא תקף. `static` גורם ל-`buf` לחיות ב-static memory לכל אורך ריצת התוכנית.

## Q3. The average bug with `int16_t sum` is not a memory bug and no sanitizer reported it. How did you find it, and how did you choose the new type?

Answer:
מצאתי אותו בקריאת הקוד: `int16_t sum` יכול להציף אם מחברים 8 ערכים של `int16_t`. שיניתי ל-`int32_t` שמספיק גדול לכל סכום אפשרי (8 × 32767 = 262136, הרבה מתחת ל-2^31).

## Q4. Why does embedded code avoid `malloc`, and why does this log use a fixed static array instead?

Answer:
`malloc` יכול להיכשל, לגרום לפרגמנטציה ולהתנהגות לא צפויה בזמן ריצה. ב-firmware צריך לדעת בזמן קומפילציה כמה RAM משתמשים. מערך סטטי מבטיח שהזיכרון תמיד זמין ואין הפתעות.
