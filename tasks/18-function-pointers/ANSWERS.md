# Task 18 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `binary_op_t op = op_add;` and `op(2, 3)`, what is stored in `op`, and what happens at the CPU level when the call runs?

Answer:
ב-`op` מאוחסנת כתובת הזיכרון של הפונקציה `op_add` — כמו כל מצביע, רק שהוא מצביע לקוד ולא לנתונים. כשמריצים `op(2, 3)`, המעבד טוען את הכתובת מ-`op`, קופץ אליה (indirect call / `call [reg]`), ומריץ את הקוד של `op_add`.

## Q2. Why is the operation table (`OP_TABLE`) better than a `switch` on the symbol when the list of commands grows? Where else do you find such tables in firmware?

Answer:
עם `switch`, כל הוספת פעולה דורשת שינוי בקוד הdispatch. עם טבלה, מוסיפים רק שורה לטבלה — הלוגיקה של החיפוש לא משתנה. בfirmware: טבלת וקטורי הפסיקות (interrupt vector table) היא מערך של function pointers, command dispatchers ב-UART, ו-state machines שבהן כל מצב הוא function pointer.

## Q3. `int cmp(int32_t a, int32_t b) { return a - b; }` is a common comparator. Why is it wrong, and what does your version do instead?

Answer:
`a - b` גולש (overflow) כשלמשל `a = INT32_MAX` ו-`b = -1`: התוצאה שלילית במקום חיובית, ומיון מתהפך. הגרסה שלי משתמשת בהשוואות `<` ו-`>` שמחזירות -1, 0, 1 — ללא כל חשבון שיכול לגלוש.

## Q4. What happens if a function pointer is NULL (or holds garbage) and you call it? Why is this especially nasty on a microcontroller?

Answer:
קריאה דרך NULL pointer גורמת ל-undefined behavior — בדרך כלל קפיצה לכתובת 0 או לכתובת אקראית. במחשב זה crash. במיקרוקונטרולר, כתובת 0 היא לרוב תחילת ה-flash (קוד אמיתי) — הקוד ממשיך לרוץ מנקודה שגויה, ללא שגיאה גלויה, ועלול לשלוט בחומרה בצורה לא צפויה.
