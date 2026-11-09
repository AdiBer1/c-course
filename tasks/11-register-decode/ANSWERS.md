# Task 11 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. The temperature field 0xF60 means -160, not 3936. Explain two's complement sign extension for a 12-bit field and how your code does it.

Answer:
ב-two's complement, ביט 11 הוא ביט הסימן. 0xF60 = 0b111101100000 — ביט 11 הוא 1, כלומר שלילי. הרחבת סימן: ממלאים את הביטים 12-15 ב-1 → 0xFF60 = -160 כ-int16_t. הקוד בודק אם ביט 11 דלוק ואם כן עושה OR עם 0xF000.

## Q2. Why does `status_read` take a `const volatile uint32_t *`? What could the compiler do with the register access if `volatile` were missing?

Answer:
`volatile` אומר לקומפיילר לא לאחסן את הערך ב-cache ולקרוא מהזיכרון בכל פעם. בלי `volatile`, הקומפיילר עלול לקרוא פעם אחת ולהשתמש בערך שמור — ולפספס שינויים שהחומרה עשתה.

## Q3. Why must the register be read only once in `status_read`, even though decoding needs several fields?

Answer:
רגיסטר חומרה יכול להשתנות בין קריאה לקריאה (למשל ISR שמשנה אותו). אם קוראים פעמיים, שדות שונים עלולים להגיע מרגעים שונים — מה שנותן מצב לא עקבי.

## Q4. What does `*(volatile uint32_t *)0x40021000u` mean, piece by piece? Why does this work on a microcontroller but crash on your PC?

Answer:
`0x40021000u` — כתובת. `(volatile uint32_t *)` — cast לפוינטר. `*` — dereference, קריאת הערך. על MCU כתובת זו מפה לרגיסטר חומרה. על PC היא לא ממופה לזיכרון תקף — גישה אליה גורמת ל-segfault.
