## Q1. In `binary_op_t op = op_add;` and `op(2, 3)`, what is stored in `op`, and what happens at the CPU level when the call runs?

Answer:
`op` מחזיק את הכתובת של הפונקציה `op_add` בזיכרון התוכנית, ולא את הקוד עצמו. בזמן הקריאה המעבד טוען את הכתובת הזאת ומבצע קפיצה אליה, בעזרת פקודת קריאה עקיפה. הארגומנטים מועברים לפי מוסכמת הקריאה, והפונקציה מחזירה את הערך לאותו מקום שממנו נקראה.

## Q2. Why is the operation table (`OP_TABLE`) better than a `switch` on the symbol when the list of commands grows? Where else do you find such tables in firmware?

Answer:
טבלה היא מידע, ולכן מוסיפים פקודה חדשה רק על ידי הוספת שורה, בלי לשנות את הקוד של החיפוש. `switch` דורש לערוך את הפונקציה בכל פעם. בקושחה נמצאות טבלאות כאלה בטבלת הוקטורים של פסיקות, בטבלאות של פקודות לתקשורת, ובממשקי מנהלי התקן עם מצביעים לפונקציות.

## Q3. `int cmp(int32_t a, int32_t b) { return a - b; }` is a common comparator. Why is it wrong, and what does your version do instead?

Answer:
ההפרש `a - b` עלול לעבור את טווח `int32_t` כשהערכים גדולים בקיצוניות, למשל `INT32_MIN` ו-`1`. בגלל זה התוצאה יכולה להיות שגויה, או לגרום להתנהגות לא מוגדרת. הגרסה שלי משווה עם `<` ו-`>` ולא מחשבת הפרש, לכן היא לא עוברת את הטווח.

## Q4. What happens if a function pointer is NULL (or holds garbage) and you call it? Why is this especially nasty on a microcontroller?

Answer:
קריאה דרך מצביע NULL מנסה לקפוץ לכתובת 0, ומצביע עם ערך זבל מקפץ לכתובת אקראית. במחשב רגיל זה יגרום לקריסה עם הודעה. במיקרו-בקר אין הגנה ממשית על הזיכרון, לכן הקוד בכתובת האקראית עלול לרוץ כקוד, ואז התנהגות המערכת לא צפויה, ולעיתים קשה לאתר את הסיבה.