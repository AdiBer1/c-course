# Task 03 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `9 / 5 * c10` and `c10 * 9 / 5` look the same in math. Why do they give different results in C, and which one is right?

Answer:
ב-C חלוקת שלמים מתבצעת לפני הכפל. `9 / 5` נותן 1 (כי 9/5=1.8 ומתקצר ל-1), ואז `1 * c10 = c10`. לעומת זאת `c10 * 9 / 5` כופל קודם ואז מחלק — זה הנכון.

## Q2. Explain how you round to the nearest integer for a negative result such as -41.8. Why does the trick "add half the divisor" alone fail for negative numbers?

Answer:
הוספת חצי המחלק עובדת לחיוביים: `(num + divisor/2) / divisor`. אבל לשליליים C מקצץ לכיוון אפס, אז -41.8 הופך ל-41 ולא ל-42. הפתרון: לחיוביים מוסיפים חצי, לשליליים מחסירים חצי — כך הקיצוץ תמיד מתנהג כעיגול לקרוב.

## Q3. On an 8-bit AVR, `int` is 16 bits. What would happen in `adc_to_mv` if the multiplication `raw * 3300` were done in `int` there? How did you make your code safe on such a chip?

Answer:
`raw * 3300` יכול להגיע עד `4095 * 3300 = 13,513,500` — גדול בהרבה מ-32767 (מקסימום int של 16 ביט). זה overflow. השתמשתי ב-`uint32_t` לכפל כדי להבטיח שהחישוב בטוח על כל פלטפורמה.

## Q4. Why would firmware on a small microcontroller avoid `float` for this kind of conversion?

Answer:
מיקרוקונטרולרים קטנים כמו Cortex-M0 או AVR אין להם יחידת נקודה צפה (FPU). חישוב float מתבצע בתוכנה ועולה מאות מחזורי שעון ותופס הרבה flash. חישוב שלמים מהיר פי עשרות ומשתמש בהרבה פחות זיכרון.

