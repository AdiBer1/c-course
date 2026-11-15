# Task 15 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simpler debounce is: "when the input changes, wait 20 ms with a delay loop, then read it again". Why is that a bad idea in firmware?

Answer:
לולאת השהיה תוקעת את המעבד: במשך 20 ms הוא לא עושה כלום אחר. לא מטפלים בתקשורת, לא מעדכנים תצוגה ולא בודקים כפתורים אחרים. עם כמה כפתורים ההשהיות מצטברות. בתוך פסיקה זה גרוע עוד יותר. חוץ מזה, זה גם לא debounce אמין: אם במקרה הקריאה אחרי 20 ms נופלת על קפיצה, מקבלים תוצאה שגויה. בגישת ה-tick כל קריאה עושה מעט עבודה וחוזרת מיד, והשינוי מתקבל רק אחרי שהקלט יציב N טיקים ברצף.

## Q2. Draw (in words) the states and transitions of your debouncer. What exactly makes it ignore a 3-tick glitch when the threshold is 5?

Answer:
שני מצבים יציבים: **RELEASED** ו-**PRESSED**, ובכל אחד מהם מונה `counter`.
- במצב RELEASED: קלט released מאפס את counter ל-0. קלט pressed מעלה את counter ב-1, וכשהוא מגיע ל-threshold עוברים ל-**PRESSED**, מחזירים `PRESS`, ‏`presses++` ו-counter = 0.
- במצב PRESSED: קלט pressed מאפס את counter ל-0. קלט released מעלה את counter ב-1, וכשהוא מגיע ל-threshold עוברים ל-**RELEASED**, מחזירים `RELEASE` ו-counter = 0.

גליץ' של 3 טיקים מעלה את counter ל-3. בטיק הרביעי הקלט חוזר להסכים עם המצב היציב, ו-counter **מתאפס** ל-0, כך שהוא אף פעם לא מגיע ל-5. האיפוס הזה הוא מה שדורש 5 טיקים **ברצף**.

## Q3. Why is all the debouncer's state inside a `debounce_t` struct passed by pointer, instead of `static` variables in debounce.c?

Answer:
כך אפשר לנהל כמה כפתורים בלתי תלויים באותו קוד: לכל כפתור יש `debounce_t` משלו (למשל מערך `debounce_t buttons[4]`). עם משתני `static` היה מצב אחד בלבד, ושני כפתורים היו מערבבים את המונים. זה גם מקל על בדיקות, כי כל בדיקה מאתחלת struct חדש ולא נשאר מצב מבדיקה קודמת, וזה הופך את הקוד ל-reentrant, כלומר לא תלוי במצב גלובלי נסתר.

## Q4. With a 1 ms tick and threshold 5, what is the delay between the contact settling and the PRESS event? How would you choose the threshold for a real button?

Answer:
אחרי שהמגע מתייצב צריך 5 טיקים רצופים של אותו ערך, כלומר כ-5 ms (בין 4 ל-5 ms, תלוי מתי ביחס לטיק המגע התייצב). כדי לבחור threshold מודדים בסקופ, או קוראים ב-datasheet, כמה זמן הכפתור הספציפי קופץ. לרוב זה 1 עד 10 ms, ובכפתורים זולים או ישנים עד 20 ms. בוחרים ערך שגדול מזמן הקפיצה המקסימלי עם מרווח ביטחון, אבל קטן מספיק כדי שהמשתמש לא ירגיש עיכוב: עד 20 עד 50 ms זה מורגש כמיידי. בפועל, עם tick של 1 ms, ‏threshold של 10 עד 20 הוא בחירה טובה.
