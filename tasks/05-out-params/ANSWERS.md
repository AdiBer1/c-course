# Task 05 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why can't `void swap_i32(int32_t a, int32_t b)` (without pointers) swap the caller's variables? Explain what is copied where.

Answer:
ב-C כל הפרמטרים מועברים by value: בקריאה נוצרים עותקים חדשים של הערכים במסגרת המחסנית (stack frame) של הפונקציה. הפונקציה מחליפה בין שני העותקים האלה, וכשהיא חוזרת המסגרת שלה נעלמת, והמשתנים של הקורא לא השתנו בכלל. עם מצביעים מועתקות **הכתובות** של המשתנים, ודרך `*a` ו-`*b` הפונקציה כותבת ישירות לזיכרון של הקורא.

## Q2. Why does `min_max_i16` take `const int16_t *values` but `int16_t *min_out` without `const`? What would the compiler do if you tried to write `values[0] = 0;`?

Answer:
`values` הוא קלט בלבד: ה-`const` מבטיח לקורא שהפונקציה רק קוראת את המערך ולא משנה אותו, ומאפשר להעביר גם מערך קבוע. `min_out` ו-`max_out` הם פלטים, הפונקציה חייבת לכתוב אליהם, ולכן הם לא `const`. אם ננסה לכתוב `values[0] = 0;` הקומפיילר ייתן שגיאת קומפילציה (assignment of read-only location), כלומר הטעות נתפסת עוד לפני שהקוד רץ.

## Q3. The rule "on error, do not write to the outputs" seems strict. Describe a situation in firmware where breaking it causes a real bug.

Answer:
נניח שהקוד שומר את קריאת החיישן האחרונה התקינה ב-`last_temp`, וקורא `sensor_read(&last_temp)`. אם יש תקלה בבאס והפונקציה כותבת זבל ל-`last_temp` לפני שהיא מחזירה שגיאה, הערך התקין הקודם נדרס. אם יש מקום אחר בקוד ששוכח לבדוק את הסטטוס (או שבודק אבל ממשיך להשתמש בערך הקודם), למשל בקרת מאוורר או חימום, הוא יפעל לפי ערך שגוי ויכול להדליק או לכבות ציוד בלי סיבה. כשהפלט לא נוגע בשגיאה, הקורא תמיד מחזיק ערך ישן אבל אמיתי.

## Q4. Why do embedded APIs usually return a status code and put the result in an output parameter, instead of returning the result and using a special value like -1 for errors?

Answer:
לעתים קרובות כל הערכים האפשריים של התוצאה חוקיים: למשל `-1` הוא טמפרטורה תקינה, ו-`0xFFFF` הוא קריאת ADC אפשרית, כך שאין ערך "מיוחד" פנוי לסמן שגיאה. קוד סטטוס נפרד גם מאפשר להבחין בין סוגי שגיאות (NULL, timeout, חלוקה באפס), וזו מוסכמה אחידה בכל ה-API, כך שקל לבדוק ולהעביר שגיאות הלאה. בנוסף אפשר להחזיר כמה תוצאות בבת אחת (כמו min וגם max).
