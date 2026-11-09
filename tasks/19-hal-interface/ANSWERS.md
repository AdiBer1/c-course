# Task 19 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Your `led.c` never includes a hardware header or touches a register. What would you need to write to run it on a real board, and what in `led.c` would change?

Answer:
צריך לכתוב מימוש של `hal_gpio_t` שמתאים לבורד הספציפי — פונקציות `write` ו-`read` שכותבות/קוראות את רגיסטרי ה-GPIO של המיקרוקונטרולר. ב-`led.c` עצמו לא משתנה שום דבר — זה כל הרעיון של HAL.

## Q2. What is `ctx` for? Why can't `fake_write` just use a global `fake_gpio_t`?

Answer:
`ctx` מאפשר לאותה פונקציה לשרת מספר instances שונים. אם `fake_write` הייתה משתמשת בגלובל, לא ניתן היה להריץ שני מוקים במקביל (למשל שני LEDs על שני GPIOs שונים) — שניהם היו כותבים לאותו גלובל ומתערבבים. עם `ctx`, כל instance מחזיק את המצב שלו.

## Q3. Why are the teacher's mock and your fake both useful? What does testing the LED driver on a mock give you that testing on the real board doesn't?

Answer:
מוק של המורה בודק שה-LED driver קורא לפונקציות הנכונות בסדר הנכון — בלי חומרה בכלל. זה מהיר, דטרמיניסטי, ורץ ב-CI. על הבורד האמיתי קשה לבדוק אוטומטית אם הפין עלה/ירד בזמן הנכון. הfake שלי מאפשר לבדוק גם את ה-fake עצמו — שהוא מתנהג נכון כ-HAL.

## Q4. What does "active low" mean electrically, and why does the driver handle it instead of the code that calls `led_set`?

Answer:
Active low אומר שה-LED נדלק כשהפין ב-LOW (0V) וכבה כשהפין ב-HIGH. זה נפוץ כי מעגלים רבים מחברים LED בין הפין ל-VCC. הdriver מטפל בזה כדי שהקוד שמשתמש ב-LED יחשוב רק "דלוק/כבוי" — ולא יצטרך לדעת פרטי חיווט. ההפשטה נשמרת במקום אחד.
