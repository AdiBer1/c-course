# Task 10 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `#define BIT(n) (1 << (n))` wrong for n = 31, while `((uint32_t)1u << (n))` is fine? What did UBSan or the compiler tell you?

Answer:
`1` הוא `int` בגודל 32 ביט. הזזה שמאלה של ביט לתוך ביט הסימן של `int` היא undefined behaviour. `(uint32_t)1u` הוא unsigned ולכן `<< 31` חוקי לחלוטין.

## Q2. What goes wrong with `BIT_SET(reg, i++)` (or with an unparenthesised macro like `#define BIT(n) 1u << n` used as `BIT(1 + 2)`)? Why do functions not have these problems?

Answer:
`i++` יוערך פעמיים בתוך המאקרו — פעם אחת לכל הופעה של `n`. ללא סוגריים, `BIT(1+2)` יהפוך ל-`1u << 1+2` שמחושב כ-`(1u<<1)+2=4`. פונקציות מעריכות ארגומנטים פעם אחת בלבד.

## Q3. Explain `FIELD_SET` step by step for `FIELD_SET(0x12345678, 16, 8, 0xAB)`. Why must it first clear the field?

Answer:
1. `FIELD_MASK(16,8)` = `0x00FF0000`. 2. `reg & ~mask` = `0x12005678` (מנקה את הביטים 16-23). 3. `0xAB << 16` = `0x00AB0000`. 4. OR ביניהם = `0x12AB5678`. חייבים לנקות קודם כי OR לא יכול לאפס ביטים.

## Q4. `-Wconversion` warned about `return (x << 4) | (x >> 4);` for a `uint8_t x`. Why, if the value "obviously" fits?

Answer:
`x << 4` מחושב כ-`int` (integer promotion). התוצאה היא `int`, וההחזרה ל-`uint8_t` היא narrowing conversion שעלולה לאבד ביטים. הפתרון: cast מפורש ל-`uint8_t`.
