# Task 15 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simpler debounce is: "when the input changes, wait 20 ms with a delay loop, then read it again". Why is that a bad idea in firmware?

Answer:
לולאת השהיה חוסמת את המעבד במשך 20 ms, ולכן הקוד לא יכול לעשות שום דבר אחר באותו זמן. לחיצות שמגיעות בזמן ההמתנה עלולות להיפספס, וכל כפתור יצריך השהיה משלו. הפתרון של הקורס לא חוסם: כל קריאה ל-`debounce_update` מבצעת tick אחד של עבודה, ולכן אפשר לקרוא לה מטיימר ולטפל בהרבה כפתורים במקביל.


## Q2. Draw (in words) the states and transitions of your debouncer. What exactly makes it ignore a 3-tick glitch when the threshold is 5?

Answer:
יש שני מצבים יציבים, PRESSED ו-RELEASED, ומשתנה `counter` שסופר כמה ticks ברצף הקלט הגולמי שונה מהמצב היציב. כשהקלט שווה למצב היציב, `counter` מתאפס. כשהוא שונה, `counter` עולה, ורק כשהוא מגיע ל-threshold המצב היציב מתעדכן. בהפרעה של 3 ticks, `counter` מגיע ל-3 בלבד, ואז הקלט חוזר למצב היציב לפני שהגיע ל-5, לכן `counter` מתאפס והשינוי לא מתקבל.


## Q3. Why is all the debouncer's state inside a `debounce_t` struct passed by pointer, instead of `static` variables in debounce.c?

Answer:
כל כפתור מקבל מבנה משלו, ולכן כמה כפתורים יכולים לפעול בלי להפריע זה לזה. משתני `static` היו משותפים לכל הקריאות, כלומר כפתור אחד היה משפיע על השני. בנוסף, מבנה שנשלח במצביע מאפשר לבדוק כל מקרה בנפרד, כי לכל בדיקה יש מבנה טרי משלה, והמבנה יכול להיות גם סטטי וגם על המחסנית, לפי החלטת מי שקורא לו.


## Q4. With a 1 ms tick and threshold 5, what is the delay between the contact settling and the PRESS event? How would you choose the threshold for a real button?

Answer:
עם tick של 1 ms ו-threshold של 5, האירוע נרשם אחרי כ-5 ms מרגע שהקלט התייצב, כי צריך 5 ticks ברצף של קלט חדש. לבחירת threshold לכפתור אמיתי צריך להסתכל על משך הקפיצות שלו, בדרך כלל כמה ms עד עשרות ms, ולבחור ערך שגדול ממשך הרעש הארוך ביותר, אבל לא גדול מדי, כי הוא מוסיף השהיה לכל לחיצה.


