# Task 06 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `char buf[4]; strcpy(buf, "LED=1");` compiles without error. What exactly happens in memory, and why can it crash much later in a completely different function?

Answer:
`buf` מקצה 4 בייטים אבל "LED=1" דורש 6 (5 תווים + '\0'). `strcpy` כותב מעבר לגבול המערך ודורס על זיכרון שליד אחר. הקריסה עלולה מאוחר כי הזיכרון הפגוע שייך למשתנה אחרת או לכתובת חזרה של פונקציה אחרת.

## Q2. Why does `my_strlcpy` return the length of `src` rather than the number of bytes copied? Show how a caller uses that value.

Answer:
כדי שהקורא יוכל לגלות אם הייתה חיתוך: `if (my_strlcpy(buf, s, sizeof buf) >= sizeof buf) { /* truncated */ }`. אם החזירה הייתה מספר הבייטים שהועתקו, אי אפשר היה לדעת אם המקור היה ארוך יותר.

## Q3. Why must `my_strcmp` compare characters as `unsigned char`? Give an example where using plain `char` gives the wrong order.

Answer:
במערכות רבות `char` הוא signed ותווים מעל 127 (כמו תווים UTF-8) יהפכו לשליליים. לדוגמה: '\x80' כ-signed char הוא -128, ויסודר לפני 'A' (65) במקום אחריו. כ-unsigned char הוא 128 ויסודר אחרי 'A' בצורה נכונה.

## Q4. `sizeof buf` and `my_strlen(buf)` for `char buf[16] = "hi";` — what are the two values and why are they different?

Answer:
`sizeof buf` = 16 — גודל המערך בבייטים, קבוע בזמן קומפילציה. `my_strlen(buf)` = 2 — מספר התווים עד '\0', נקבע בזמן ריצה. ההבדל: `sizeof` מודד זיכרון, `strlen` מודד תוכן.

