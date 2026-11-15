## Q1. What can go wrong if the "lines ready" counter is not `volatile`? Describe what the optimiser is allowed to do with `while (!uart_rx_line_ready()) { }` in the main loop.

Answer:
בלי `volatile`, הקומפיילר רואה שהלולאה לא משנה את המשתנה, ולכן מותר לו לקרוא את הערך פעם אחת לרגיסטר ולהשתמש בו שוב ושוב. אם הערך היה 0 בתחילה, הלולאה תיתקע לנצח, גם אחרי שה-ISR כבר עדכן את המשתנה בזיכרון. `volatile` אומר לקומפיילר לקרוא את הזיכרון בכל גישה.

## Q2. Your main loop does `count--` and the ISR does `count++`. Explain step by step how an interrupt at the wrong moment loses an update if there is no critical section.

Answer:
הלולאה הראשית קוראת את `count`, למשל הערך 5, ולפני שהיא כותבת את התוצאה 4, מגיעה פסיקה. ה-ISR קורא את 5, מוסיף אחד, וכותב 6. כשהפסיקה מסתיימת, הלולאה הראשית ממשיכה עם הערך הישן שכבר קראה, מחשבת 4, וכותבת אותו. העדכון של ה-ISR נמחק, והספירה לא מדויקת.

## Q3. Why must the critical section be short, and why must the ISR itself be short? What would happen to incoming bytes otherwise?

Answer:
בזמן שהפסיקות כבויות, בייטים שמגיעים מהחוץ לא מטופלים. אם הקטע הקריטי ארוך, בייט נוסף יכול להגיע לפני שקראנו את הקודם מרגיסטר הנתונים, והבייט הקודם יאבד. אותו דבר קורה אם ה-ISR איטי, כי הוא לא יספיק לקרוא את הבייט הבא בזמן.

## Q4. Why does the ISR read the data register even when the buffer is full and the byte will be dropped?

Answer:
על חומרה אמיתית הפסיקה נשארת פעילה כל עוד רגיסטר הנתונים מלא ולא נקרא. אם ה-ISR לא קורא את הבייט, הפסיקה תמשיך לחזור, והבייט הבא ידרוס אותו. לכן ה-ISR קורא תמיד, גם כשהוא מחליט לזרוק את הבייט.