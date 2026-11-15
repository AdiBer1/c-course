# Task 14 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. If you only had `head` and `tail` (no `count`), `head == tail` could mean "empty" or "full". How does the struct here solve that, and what is the other common solution?

Answer:
ה-struct מחזיק שדה `count`, שעולה ב-push ויורד ב-pop. ריק הוא `count == 0`, ומלא הוא `count == size`, בלי עמימות, וכל `size` הבתים שמישים. הפתרון הנפוץ השני הוא להשאיר תמיד תא אחד ריק: "מלא" מוגדר כ-`(head + 1) % size == tail` ו"ריק" כ-`head == tail`. כך לא צריך `count`, אבל אפשר לאחסן רק `size - 1` בתים. (פתרון שלישי הוא אינדקסים שרצים בלי לעשות wrap, ומחשבים `head - tail`.)

## Q2. Many implementations require the size to be a power of two and use `index & (size - 1)` instead of a comparison or `%`. Why is that popular on microcontrollers?

Answer:
`%` דורש חילוק, ובהרבה מיקרו-בקרים (Cortex-M0, AVR) אין פקודת חילוק בחומרה, אז זו קריאה לפונקציית ספרייה איטית של עשרות מחזורים. כשהגודל הוא חזקה של 2, `index & (size - 1)` נותן את אותה תוצאה בפקודת AND אחת, בלי הסתעפות (branch) ובזמן קבוע. זה חשוב במיוחד בתוך פסיקה, שצריכה להיות קצרה ככל האפשר. ‏(כאן בחרנו השוואה ל-`size` כי הגודל יכול להיות כל מספר.)

## Q3. In a UART driver, the receive interrupt pushes and the main loop pops. Which fields does each side write? Why can this be dangerous with the `count` design, and what would you do about it?

Answer:
הפסיקה (push) כותבת את `head` ואת `count`, והלולאה הראשית (pop) כותבת את `tail` ואת `count`. הבעיה היא ש-`count` נכתב **משני הצדדים**: `count--` בלולאה הוא בעצם read-modify-write (קריאה, חיסור, כתיבה). אם הפסיקה קופצת באמצע ועושה `count++`, הלולאה כותבת אחר כך ערך ישן, ופסיקה אחת "נבלעת": count שגוי, ובאפר שנראה ריק או מלא בטעות. פתרונות: לכבות פסיקות סביב עדכון `count` בצד של הלולאה (critical section קצר), או לוותר על `count` ולהשתמש בפתרון "תא ריק אחד", שבו כל צד כותב רק אינדקס אחד (`head` או `tail`). השדות המשותפים צריכים להיות `volatile`, וקריאה וכתיבה שלהם צריכות להיות אטומיות.

## Q4. Why does the caller provide the storage array instead of the ring buffer having a fixed internal array?

Answer:
כך מימוש אחד משרת באפרים בכל גודל: ‏RX של UART של 256 בתים, לוג של 64, תור פקודות של 16. אם המערך היה בתוך המודול, היה צריך גודל קבוע אחד לכולם (בזבוז או מחסור), או לשכפל את הקוד. הקורא גם מחליט איפה הזיכרון יושב (static, אזור RAM מסוים, מיושר ל-DMA), וכל הזיכרון מוקצה סטטית ונראה בדו"ח הלינקר, בלי `malloc`.
