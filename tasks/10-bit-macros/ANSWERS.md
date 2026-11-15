# Task 10 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `#define BIT(n) (1 << (n))` wrong for n = 31, while `((uint32_t)1u << (n))` is fine? What did UBSan or the compiler tell you?

Answer:
הליטרל `1` הוא `int` עם סימן, בדרך כלל 32 ביט. ‏`1 << 31` מזיז 1 לתוך ביט הסימן, והתוצאה (2³¹) לא ניתנת לייצוג ב-int, וזה undefined behaviour. ‏UBSan מדווח על זה כ-"left shift of 1 by 31 places cannot be represented in type 'int'". עם `(uint32_t)1u` ההזזה נעשית על טיפוס unsigned של 32 ביט, שבו 0x80000000 הוא ערך חוקי ומוגדר היטב. כל הזזה מ-0 עד 31 בטוחה.

## Q2. What goes wrong with `BIT_SET(reg, i++)` (or with an unparenthesised macro like `#define BIT(n) 1u << n` used as `BIT(1 + 2)`)? Why do functions not have these problems?

Answer:
מאקרו הוא החלפת טקסט. אם הפרמטר מופיע בגוף המאקרו יותר מפעם אחת, `i++` יתבצע כמה פעמים, ויכול להיות גם UB (כמה שינויים בלי sequence point). בלי סוגריים, `BIT(1 + 2)` הופך ל-`1u << 1 + 2`, ומכיוון ש-`+` קודם ל-`<<` זה במקרה עובד, אבל `~BIT(3)` הופך ל-`~1u << 3`, כלומר `(~1u) << 3`, וזה ערך שגוי לגמרי. פונקציה מחשבת כל ארגומנט בדיוק פעם אחת לפני הקריאה ומקבלת ערך ולא טקסט, כך שסדר קדימויות ותופעות לוואי לא משנים.

## Q3. Explain `FIELD_SET` step by step for `FIELD_SET(0x12345678, 16, 8, 0xAB)`. Why must it first clear the field?

Answer:
1. `FIELD_MASK(16, 8)` = ‏`0xFFFFFFFF >> 24` = ‏`0xFF`, ואחרי הזזה ב-16 מקבלים `0x00FF0000`.
2. ניקוי השדה: `0x12345678 & ~0x00FF0000` = ‏`0x12345678 & 0xFF00FFFF` = ‏`0x12005678`.
3. הכנסת הערך: `(0xAB << 16) & 0x00FF0000` = ‏`0x00AB0000`. ה-`&` זורק ביטים של val שלא נכנסים בשדה.
4. OR: ‏`0x12005678 | 0x00AB0000` = ‏`0x12AB5678`.

חייבים לנקות קודם, כי OR יכול רק להדליק ביטים ולא לכבות. בלי ניקוי, `0x34 | 0xAB` = ‏`0xBF` במקום `0xAB`.

## Q4. `-Wconversion` warned about `return (x << 4) | (x >> 4);` for a `uint8_t x`. Why, if the value "obviously" fits?

Answer:
לפני חישוב, `uint8_t` עובר integer promotion ל-`int`, ולכן כל הביטוי הוא `int`. ‏`x << 4` יכול להגיע עד ‎0xFF0‎, שבאמת לא נכנס ב-8 ביט. זו בדיוק הסיבה שהקומפיילר מזהיר שהחזרה ל-`uint8_t` חותכת ביטים. אנחנו **רוצים** את החיתוך הזה, כי הביטים העליונים מיותרים, אז מבטאים את זה במפורש עם `& 0xFF` ו-cast ל-`(uint8_t)`. ה-cast אומר לקורא ולקומפיילר שהצמצום מכוון.
