# Task 05 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why can't `void swap_i32(int32_t a, int32_t b)` (without pointers) swap the caller's variables? Explain what is copied where.

Answer:
ב-C פרמטרים מועברים by value — הפונקציה מקבלת עותקים של המשתנים. החלפת העותקים בתוך הפונקציה לא משפיעה על המשתנים המקוריים של הקורא. עם מציינים, הפונקציה מקבלת כתובת זיכרון ויכולה לשנות את הערך במקום המקורי.

## Q2. Why does `min_max_i16` take `const int16_t *values` but `int16_t *min_out` without `const`? What would the compiler do if you tried to write `values[0] = 0;`?

Answer:
`const int16_t *values` אומר שהפונקציה לא תשנה את המערך — הוא קלט בלבד. `min_out` הוא פלט ולכן אין `const`. אם ננסה לכתוב `values[0] = 0`, הקומפיילר יוציא שגיאת קומפילציה.

## Q3. The rule "on error, do not write to the outputs" seems strict. Describe a situation in firmware where breaking it causes a real bug.

Answer:
דמייני: פונקציית ADC שמחזירה שגיאה אבל עדיין כותבת 0 למשתנה המאחסן את ערך החיישאן. הקוד הקורא בודק את קוד השגיאה אבל עדיין משתמש בערך 0 המזויף בתור קריאה תקינה — והמערכת מתנהגת לפי ערך שגוי.

## Q4. Why do embedded APIs usually return a status code and put the result in an output parameter, instead of returning the result and using a special value like -1 for errors?

Answer:
לא תמיד יש ערך מיוחד שאפשר להשתמש בו לשגיאה. לדוגמה, `uint16_t` יכול להחזיר כל ערך בין 0 ל-65535 — אין מקום ל-1. קוד סטטוס נפרד מאפשר לבדוק הצלחה/כשלון באופן ברור בלי לוותר על טווח הערכים.

