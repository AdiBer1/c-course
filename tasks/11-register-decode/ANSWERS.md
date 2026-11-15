# Task 11 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. The temperature field 0xF60 means -160, not 3936. Explain two's complement sign extension for a 12-bit field and how your code does it.

Answer:
בשדה של 12 ביטים, הביט הגבוה ביותר (ביט 11) הוא ביט הסימן. אם הוא דולק, המספר שלילי, והערך האמיתי הוא המספר הגולמי פחות 0x1000 (2^12). לכן 0xF60 = 3936, ו-3936 - 4096 = -160. בקוד, אני קורא את השדה עם מסכה של 0xFFF, ואם ביט 11 דולק (0x800) אני מחסיר 0x1000, ואז שומר את התוצאה ב-int16_t.

## Q2. Why does `status_read` take a `const volatile uint32_t *`? What could the compiler do with the register access if `volatile` were missing?

Answer:
ה-volatile אומר לקומפיילר שהערך בזיכרון יכול להשתנות בכל רגע על ידי החומרה, לכן אסור לו לשמור אותו ברגיסטר או להניח שהוא לא השתנה. בלי volatile, הקומפיילר יכול לקרוא את הרגיסטר פעם אחת ולשמור את התוצאה, או אפילו לדלג על הקריאה לגמרי אם לא רואה שימוש בערך. ה-const מסמל שהפונקציה לא כותבת לרגיסטר, רק קוראת ממנו.

## Q3. Why must the register be read only once in `status_read`, even though decoding needs several fields?

Answer:
החומרה יכולה לשנות את הרגיסטר בין קריאה לקריאה. אם הייתי קורא אותו פעמיים (למשל פעם אחת עבור המצב ופעם אחת עבור הטמפרטורה), הייתי מקבל שדות מתוך שני רגעים שונים, וערך לא עקבי. לכן קוראים את הערך פעם אחת לתוך משתנה מקומי, ומפרקים את השדות מהעותק הזה.

## Q4. What does `*(volatile uint32_t *)0x40021000u` mean, piece by piece? Why does this work on a microcontroller but crash on your PC?

Answer:
`0x40021000u` הוא כתובת מספרית בלבד. `(volatile uint32_t *)` ממיר אותה למצביע לערך של 32 ביט שהקומפיילר לא יאפטם. הכוכבית מבקשת את הערך שבכתובת הזו. על המיקרו-בקר, הכתובת הזו ממופה לרגיסטר אמיתי של החומרה, לכן הקריאה מחזירה את הערך של הרגיסטר. במחשב, הכתובת הזו לא שייכת לתהליך שלי, ולכן מערכת ההפעלה עוצרת את התוכנית (segmentation fault).