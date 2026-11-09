# Task 16 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why must the parser check `LEN > PKT_MAX_PAYLOAD` before storing any payload byte? What would a corrupted LEN of 200 do otherwise?

Answer:
הבאפר `payload[]` מוגדר בגודל `PKT_MAX_PAYLOAD` (32 בתים). אם LEN=200 ולא נבדוק, הפארסר ינסה לכתוב 200 בתים לתוך מערך של 32 — buffer overflow שיכתוב מעבר לגבולות ה-struct בזיכרון. בדיקת LEN לפני כל כתיבה מונעת זאת ומחזירה שגיאה.

## Q2. Bytes are lost on the wire: the receiver misses the LEN byte of a frame. Walk through what your state machine does and how it gets back in sync.

Answer:
אם LEN אבד, הפארסר מקבל את הבייט הראשון של ה-payload כ-LEN. ערך זה כנראה לא יתאים לאורך האמיתי, ולכן ה-checksum יהיה שגוי — הפארסר יחזיר `PKT_ERR_CHECKSUM` ויחזור ל-`WAIT_START`. משם הוא ממתין ל-`0x7E` הבא ומתחיל מחדש. כך הוא מסתנכרן מחדש עם הפריים הבא.

## Q3. Why does the parser keep a separate `done[]` copy of the last complete payload, instead of letting the caller read `payload[]` directly?

Answer:
בזמן שהקורא מעבד את ה-payload האחרון, הפארסר ממשיך לקבל בייטים ומשתמש ב-`payload[]` לאחסון הפריים הנוכחי. אם הקורא היה קורא ישירות מ-`payload[]`, הנתונים היו נדרסים תוך כדי קריאה. `done[]` הוא עותק שנשמר ולא משתנה עד ל-`PKT_COMPLETE` הבא.

## Q4. A XOR checksum is used here. What kind of error can it NOT detect, and when would you use the CRC-8 from task 13 instead?

Answer:
XOR לא מזהה החלפה של שני בייטים זה בזה (swap), ולא מזהה מספר זוגי של שגיאות באותו ביט. CRC-8 עדיף כשצריך לזהות רצפי שגיאות (burst errors) כמו בתקשורת רועשת — CRC מזהה כל שגיאה ברצף עד אורך הפולינום.
