## Q1. Your `led.c` never includes a hardware header or touches a register. What would you need to write to run it on a real board, and what in `led.c` would change?

Answer:
נצטרך לכתוב מימוש אמיתי של `hal_gpio_t`, כלומר פונקציות `write` ו-`read` שמשנות את רגיסטרי הפורט של המיקרו-בקר, ולהגדיר את הפין כיציאה או כניסה. ב-`led.c` לא ישתנה כלום, כי הוא כבר עובד דרך הממשק בלבד. מה שמשתנה הוא הקובץ שממלא את ה-struct בערכים אמיתיים.

## Q2. What is `ctx` for? Why can't `fake_write` just use a global `fake_gpio_t`?

Answer:
`ctx` הוא מצביע שמועבר בחזרה לכל קריאה, כך שמימוש אחד יכול לשרת כמה מופעים בו זמנית. משתנה גלובלי היה משותף לכל הקריאות, ולכן שני פייקים לא היו עצמאיים, והבדיקות היו מושפעות זו מזו. `ctx` מאפשר לכל מופע לשמור את המצב שלו.

## Q3. Why are the teacher's mock and your fake both useful? What does testing the LED driver on a mock give you that testing on the real board doesn't?

Answer:
ה-mock והפייק מאפשרים לבדוק את הלוגיקה של הדרייבר על המחשב, במילישניות, בצורה דטרמיניסטית, ועם sanitizers. על הלוח אי אפשר לבדוק בקלות את הרצף המדויק של הכתיבות, או לשלוט בערך הכניסה. מצד שני, הלוח מגלה דברים שה-mock לא יכול לגלות, כמו חיווט, תזמון אמיתי והתנהגות חשמלית.

## Q4. What does "active low" mean electrically, and why does the driver handle it instead of the code that calls `led_set`?

Answer:
Active low אומר שהנורה מחוברת כך שהפין במצב LOW מספק זרם דרכה, ולכן LOW מדליק אותה. הדרייבר מטפל בזה כדי שכל שאר הקוד יחשוב רק במונחים של דלוק וכבוי. אם נשנה את החיווט בלוח, נשנה דגל אחד בלבד, ולא נצטרך לעבור על כל הקריאות ל-`led_set`.