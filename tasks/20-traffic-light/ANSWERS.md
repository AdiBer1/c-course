## Q1. Why is `if (now >= tl->phase_start_ms + TL_RED_MS)` wrong near the 32-bit wrap-around, while `if ((uint32_t)(now - start) >= TL_RED_MS)` is right? Use concrete numbers.

Answer:
נניח שההתחלה היא 0xFFFFFFF0, כלומר 4294967280, ושלב האדום הוא 5000 ms. הסכום `start + 5000` עובר את 2^32 ומתכנס ל-4984. אם ה-now הוא 4294967396, כלומר 100 ms אחרי ההתחלה, ההשוואה `now >= 4984` תחזיר אמת, והשלב ייגמר מיד בטעות. לעומת זאת, `(uint32_t)(now - start)` נותן 116 ms, שזה הערך הנכון, כי החיסור ב-uint32 מטפל בעטיפה באופן טבעי.

## Q2. What would happen to the rest of the firmware if `tl_update` used a delay loop to wait for the 5 seconds of RED? How does the test catch it?

Answer:
לולאת המתנה תעצור את כל המערכת במשך 5 שניות: הכפתור, ה-UART והמשימות האחרות לא יקבלו זמן מעבד, והלחיצות יפוספסו. הבדיקה תופסת את זה כי השעון במוק לא מתקדם מעצמו. לולאת המתנה תמתין לנצח, ולכן הבדיקה `test_update_without_time_passing_returns_immediately` תיכשל בזמן הקצוב של 5 שניות.

## Q3. When an update comes 300 ms late, your new phase starts at "now", not at "start + duration". What is the difference over many cycles, and when would you prefer the other choice?

Answer:
אם כל שלב מתחיל ב-"עכשיו", כל מחזור מתארך בעשרות ואף בלמעלה מ-300 ms, כלומר ההסחף מצטבר. במערכת שחייבת לשמור על תדירות מדויקת, למשל הבהוב קבוע, עדיף לחשב את ההתחלה כ-"התחלה + משך", כדי שהלוח הזמנים לא יסיט. מצד שני, אם המערכת נעצרה זמן רב, החישוב השני יגרום לרצף של שלבים שמבוצעים מיד אחד אחרי השני, ולכן ב-"עכשיו" נמנעים מזה.

## Q4. This module only uses `hal_gpio_t` and `hal_clock_t`. List exactly what changes when you move it to a real microcontroller.

Answer:
ה-`traffic_light.c` לא ישתנה. צריך לכתוב מימוש אמיתי של `hal_gpio_t` שכותב לרגיסטרי הפורט, ומגדיר את הפינים לכיוון הנכון, ומימוש של `hal_clock_t` שקורא מונה שמתעדכן בפסיקת טיימר. צריך גם לוודא שהקריאה למונה בטוחה, למשל עם `volatile` או מנגנון אטומי, ולעדכן את ה-mock בלוח במקום הבדיקות.