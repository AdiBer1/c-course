# Task 11 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. The temperature field 0xF60 means -160, not 3936. Explain two's complement sign extension for a 12-bit field and how your code does it.

Answer:
במשלים ל-2 של 12 ביט, הביט העליון (ביט 11, ערך 0x800) הוא ביט הסימן ומשקלו ‎-2048‎ במקום ‎+2048‎. לכן אם הביט דלוק, הערך האמיתי הוא הערך הגולמי פחות 2¹² = 4096. ‏`0xF60` = 3936, ביט 11 דלוק, ולכן 3936 − 4096 = ‎-160‎ (שזה ‎-10‎ מעלות). בקוד: מוציאים את 12 הביטים כ-unsigned, ממירים ל-`int32_t`, ואם `raw & 0x800` מחסירים `4096`. זה שקול להעתקת ביט הסימן לכל הביטים העליונים (sign extension) לפני ההמרה ל-`int16_t`.

## Q2. Why does `status_read` take a `const volatile uint32_t *`? What could the compiler do with the register access if `volatile` were missing?

Answer:
`volatile` אומר לקומפיילר שהערך בכתובת יכול להשתנות בלי שהקוד שלנו כתב אליו (החומרה משנה אותו), ולכן כל קריאה בקוד חייבת להיות קריאה אמיתית מהזיכרון. ‏`const` אומר שאנחנו רק קוראים ולא כותבים לרגיסטר. בלי `volatile` הקומפיילר רשאי לאחד כמה קריאות לאחת, להוציא קריאה מלולאה (ואז `while (!(*reg & READY))` הופכת ללולאה אינסופית על ערך ישן), לשנות סדר גישות, או למחוק קריאה שנראית "מיותרת".

## Q3. Why must the register be read only once in `status_read`, even though decoding needs several fields?

Answer:
החומרה יכולה לשנות את הרגיסטר בין שתי קריאות. אם קוראים פעם אחת בשביל READY ופעם נוספת בשביל FIFO_LEVEL, אפשר לקבל שילוב שלא היה קיים באף רגע: READY מהמצב הקודם ו-FIFO מהמצב החדש. בנוסף, בחלק מהרגיסטרים לקריאה עצמה יש תופעת לוואי (read-to-clear: הקריאה מאפסת דגלים), כך שקריאה שנייה מאבדת מידע. לכן קוראים פעם אחת לתוך משתנה מקומי ומפענחים ממנו את כל השדות, כמו snapshot עקבי.

## Q4. What does `*(volatile uint32_t *)0x40021000u` mean, piece by piece? Why does this work on a microcontroller but crash on your PC?

Answer:
- `0x40021000u`: מספר שלם, כתובת פיזית מתוך ה-datasheet.
- `(volatile uint32_t *)`: ממירים את המספר למצביע ל-`uint32_t` שהוא volatile.
- `*`: ניגשים (קוראים או כותבים) ל-32 הביטים בכתובת הזאת, וכל גישה נעשית בפועל.

במיקרו-בקר הכתובת הזאת ממופה לרגיסטר של פריפריה (memory-mapped I/O), ולכן הגישה מגיעה לחומרה. במחשב יש זיכרון וירטואלי: התהליך יכול לגשת רק לדפים שמערכת ההפעלה מיפתה לו, והכתובת הזאת לא ממופה, ולכן מקבלים segmentation fault.
