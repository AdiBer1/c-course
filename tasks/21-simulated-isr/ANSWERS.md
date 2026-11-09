# Task 21 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. What can go wrong if the "lines ready" counter is not `volatile`? Describe what the optimiser is allowed to do with `while (!uart_rx_line_ready()) { }` in the main loop.

Answer:
הקומפיילר רואה שהלולאה לא משנה את `lines` — ולכן מותר לו לקרוא אותו פעם אחת לרגיסטר ולהשתמש בו לנצח. הלולאה הופכת ל-`while (true)` אינסופית גם אחרי שה-ISR כתב ל-`lines`. `volatile` מכריח קריאה מחדש מהזיכרון בכל איטרציה.

## Q2. Your main loop does `count--` and the ISR does `count++`. Explain step by step how an interrupt at the wrong moment loses an update if there is no critical section.

Answer:
`count--` מתורגם ל-3 פעולות: (1) קרא `count` לרגיסטר (נניח 5). (2) חסר 1 → 4. (3) כתוב 4 חזרה. אם ה-ISR מפסיק בין (1) ל-(3) ועושה `count++` (כותב 6), אחר כך ה-main loop כותב 4 — ה-increment של ה-ISR אבד. `count` צריך להיות 5 אבל הוא 4.

## Q3. Why must the critical section be short, and why must the ISR itself be short? What would happen to incoming bytes otherwise?

Answer:
כשה-ISR רץ, פסיקות אחרות חסומות (או ה-ISR עצמו לא יכול להיקרא שוב). אם ה-ISR ארוך, בייט חדש שמגיע לא יטופל בזמן — ה-UART data register יידרס (overrun). אם ה-critical section ארוך, ה-ISR לא יכול לרוץ ובייטים יאבדו. שניהם חייבים להיות קצרים ככל האפשר.

## Q4. Why does the ISR read the data register even when the buffer is full and the byte will be dropped?

Answer:
בחומרה אמיתית, ה-UART מחזיק את הפסיקה פעילה כל עוד ה-data register לא נקרא. אם ה-ISR לא קורא אותו, הפסיקה תמשיך לירות שוב ושוב — לולאה אינסופית שחוסמת את כל הפירמוור. קריאת הרגיסטר מנקה את הפסיקה, גם אם הבייט נזרק.
