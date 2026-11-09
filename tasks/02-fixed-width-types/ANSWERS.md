# Task 02 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `uint8_t a = 200, b = 100;` what is the type and the value of the expression `a + b`? What happens when you store it back into a `uint8_t`?

Answer:
הביטוי `a + b` מוחזר כ-`int` (integer promotion) עם הערך 300. כשמאחסנים אותו ב-`uint8_t`, מתבצעת חיתוך ל-8 ביטים: 300 % 256 = 44. כלומר במקום 300 מקבלים 44 — wrap-around.

## Q2. Why does embedded code use `uint8_t`, `int16_t`, `uint32_t` instead of `char`, `short`, `int`? Give a concrete case where plain `int` would surprise you.

Answer:
כי הגודל של `char`, `short`, `int` תלוי בפלטפורמה ובקומפיילר. `uint8_t` מובטח תמיד 8 ביטים. לדוגמה: `int` יכול להיות 16 ביטים על מיקרוקונטרולר ישן ו-32 ביטים על PC — קוד שעובד על אחד עלול להישבר על השני.

## Q3. Unsigned overflow wraps around in a defined way, but signed overflow is undefined behaviour. Why is that difference dangerous, and how does your `sat_add_i16` avoid ever overflowing?

Answer:
overflow של signed מאפשר לקומפיילר לעשות אופטימיזציות שמניחות שזה לא קורה — התוצאה לא צפויה. `sat_add_i16` נמנע מזה על ידי הרחבה ל-`int32_t` לפני החיבור, כך שאין overflow בכלל, ורק אחר כך בודק ומגביל לטווח `int16_t`.

## Q4. A battery-level reading is `uint8_t level` (0–255). The code does `level - 10` to add a safety margin, and the result is stored in a `uint8_t`. What goes wrong when `level` is 5, and why does saturation fix it?

Answer:
כש-`level` הוא 5, החישוב `5 - 10` עם unsigned נותן wrap-around: 5 - 10 = 245 (במקום ערך שלילי). הקוד חושב שהסוללה כמעט מלאה! עם saturation, התוצאה תהיה 0 — הערך המינימלי הנכון.

