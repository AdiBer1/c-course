# Task 12 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `return *(const uint32_t *)p;` a bad way to implement `rd_u32_le`, even if it passes the tests on your PC? Give two separate reasons.

Answer:
1. **סדר בתים:** הקוד קורא את הבתים לפי הסדר של המעבד שעליו הוא רץ. על x86 (little-endian) זה במקרה נכון, אבל על מעבד big-endian אותו קוד יחזיר ערך הפוך. הפונקציה אמורה לקרוא little-endian תמיד, בלי תלות במעבד.
2. **יישור (alignment):** `p` יכול להצביע לכל כתובת, למשל לבית 3 בתוך פקטה. קריאת `uint32_t` מכתובת לא מיושרת היא undefined behaviour, ועל Cortex-M0 היא גורמת ל-HardFault. ‏x86 סלחני לזה, ולכן הבדיקות עוברות במחשב.

(ובנוסף, גישה ל-`uint8_t[]` דרך `uint32_t *` שוברת את כללי ה-strict aliasing, והקומפיילר רשאי לבצע אופטימיזציות שישברו את הקוד.)

## Q2. `p[3] << 24` where `p[3]` is a `uint8_t` 0xFF: what is the type of the expression, and what did UBSan report if you didn't cast? How does the cast fix it?

Answer:
`uint8_t` עובר integer promotion ל-`int` עם סימן, ולכן `p[3] << 24` הוא מטיפוס `int`. ‏`0xFF << 24` = ‏`0xFF000000`, שגדול מ-`INT_MAX`, כלומר מזיזים 1 לתוך ביט הסימן, וזה UB. ‏UBSan מדווח: "left shift of 255 by 24 places cannot be represented in type 'int'". עם `(uint32_t)p[3] << 24` ההזזה נעשית על unsigned של 32 ביט, שבו `0xFF000000` הוא ערך חוקי ומוגדר.

## Q3. A temperature sensor sends its value big-endian as bytes `FF 38`. Walk through how you get -200 from those two bytes.

Answer:
1. Big-endian: הבית הראשון הוא החשוב. ‏`(0xFF << 8) | 0x38` = ‏`0xFF38` = 65336 (כ-unsigned).
2. ביט 15 דלוק (`0xFF38 >= 0x8000`), ולכן במשלים ל-2 זה מספר שלילי: 65336 − 65536 = ‎-200‎.
3. מחזירים `(int16_t)-200`.

בקוד: `rd_i16_be` קוראת עם `rd_u16_be`, ואם הערך `>= 0x8000` מחסירה `0x10000`, כך שאין המרה מ-unsigned ל-signed שתלויה במימוש.

## Q4. Your firmware runs on a little-endian MCU and talks to a big-endian protocol. Why is it still better to call `rd_u32_be()` everywhere than to write "swap the bytes because we are little-endian"?

Answer:
`rd_u32_be()` מתאר **את הפורמט של הנתונים** ("הפרוטוקול הוא big-endian"), ולא הנחה על המעבד. הקוד נכון על כל מעבד, גם אם בעתיד עוברים לשבב אחר או מריצים את הלוגיקה בבדיקות על PC. "החלף בתים" מקודד הנחה נסתרת שנשברת בשקט אם הסביבה משתנה, וקל לשכוח החלפה או להחליף פעמיים. ‏`rd_u32_be` גם עובד על באפר לא מיושר, והשם שלו מתעד את הכוונה. קומפיילר טוב בדרך כלל ממיר אותו לפקודת `REV` אחת, כך שאין מחיר בביצועים.
