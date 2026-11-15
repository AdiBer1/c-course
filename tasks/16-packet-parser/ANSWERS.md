# Task 16 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why must the parser check `LEN > PKT_MAX_PAYLOAD` before storing any payload byte? What would a corrupted LEN of 200 do otherwise?

Answer:
המערך `payload` מוגדר בגודל 32 בלבד, ואם לא בודקים את LEN, הלולאה תכתוב בייטים מעבר לסוף המערך. זו כתיבה מחוץ לגבולות, והיא עלולה לדרוס שדות אחרים במבנה או משתנים בזיכרון, כלומר התנהגות לא מוגדרת. LEN שגוי של 200 היה גורם לכתיבה של 200 בתים לתוך מערך של 32.


## Q2. Bytes are lost on the wire: the receiver misses the LEN byte of a frame. Walk through what your state machine does and how it gets back in sync.

Answer:
הפרסר נשאר במצב WAIT_START עד שמגיע 0x7E. אם חסר בייט LEN, הבייט הבא שיגיע, שהוא מטען או checksum, יפורש כ-LEN. אם הערך גדול מ-32 מתקבלת שגיאת אורך והפרסר חוזר ל-WAIT_START. אם הערך קטן, הפרסר יקלוט כמה בייטים כמטען, ה-checksum לא יתאים, והפרסר חוזר ל-WAIT_START. ההתאוששות מגיעה כשמופיע 0x7E של החבילה הבאה, כי הפרסר מחפש תמיד התחלה חדשה.


## Q3. Why does the parser keep a separate `done[]` copy of the last complete payload, instead of letting the caller read `payload[]` directly?

Answer:
`payload[]` משתנה בזמן קבלה של חבילה חדשה, ולכן הוא מכיל לפעמים חצי חבילה או נתונים של חבילה שנכשלה. `done[]` מחזיק את החבילה התקינה האחרונה בלבד, והמתקשר יכול לקרוא ממנו בבטחה בזמן שחבילה חדשה נקלטת. כך חבילה שנכשלה לא משנה את הנתונים שהמתקשר רואה.


## Q4. A XOR checksum is used here. What kind of error can it NOT detect, and when would you use the CRC-8 from task 13 instead?

Answer:
XOR לא מזהה שגיאות שמבטלות זו את זו. למשל, אם שני בייטים מתהפכים באותו ביט, ה-XOR נשאר זהה. XOR גם לא מזהה שינוי סדר של בייטים. CRC-8 מזהה טוב יותר שגיאות רצופות ושגיאות של מספר ביטים, ולכן משתמשים בו כשהערוץ רועש או כשאמינות חשובה יותר מפשטות.

