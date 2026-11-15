# Task 13 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simple checksum adds all bytes. Give an error that a sum does not detect but a CRC does.

Answer:
החלפת סדר בתים: ‏`{0x01, 0x02}` ו-`{0x02, 0x01}` נותנים אותו סכום (3), כי חיבור הוא קומוטטיבי, כך שהשגיאה לא תתגלה. ב-CRC כל בית משפיע על המצב בצורה שתלויה במיקום שלו, ולכן הערכים שונים. דוגמה נוספת: בית אחד שעלה ב-1 ובית אחר שירד ב-1 מבטלים זה את זה בסכום, אבל CRC תופס את זה.

## Q2. Inside `crc8_update`, `crc << 1` on a `uint8_t` produced a `-Wconversion` warning until you cast. What is the type of `crc << 1`, and why is the cast correct here rather than a hack?

Answer:
`crc` עובר promotion ל-`int`, ולכן `crc << 1` הוא `int`, וערכו יכול להגיע עד ‎0x1FE‎ (9 ביטים). האלגוריתם מוגדר כ-CRC של **8 ביט**: הביט שיוצא החוצה משמאל צריך להיזרק, ובדיוק זה מה ש-`(uint8_t)` עושה (שקול ל-`& 0xFF`). אז ה-cast מבטא את הדרישה "keep crc 8 bits wide" מה-header, ולא מסתיר באג. בקוד שלי המרתי קודם ל-`uint32_t`, כדי שההזזה תהיה unsigned ולא תהיה גם אזהרת sign-conversion.

## Q3. The bit-by-bit CRC needs 8 loop iterations per byte. A table-driven version uses a 256-byte lookup table and one step per byte. When would you choose each on a microcontroller?

Answer:
**ביט אחרי ביט:** כשזיכרון ה-Flash מצומצם (מיקרו-בקר קטן עם כמה KB), כשהנתונים מעטים (כמה בתים מחיישן כל שנייה), או כשהמהירות לא קריטית. הקוד קטן ופשוט לבדיקה.
**טבלה:** כשמחשבים CRC על הרבה נתונים במהירות גבוהה (תקשורת רדיו, עדכון firmware, זרם UART מהיר) ויש 256 בתים פנויים ב-Flash. זה בערך פי 8 פחות עבודה לכל בית. הטבלה היא `const`, ולכן נשמרת ב-Flash ולא ב-RAM. (אם יש יחידת CRC בחומרה, לרוב עדיף להשתמש בה.)

## Q4. Why does the receiver compute the CRC over the data and compare, instead of the sender just sending the data twice?

Answer:
שליחה כפולה מכפילה את גודל ההודעה (‎100%‎ תוספת), בעוד ש-CRC-8 מוסיף בית אחד בלבד. בנוסף, שליחה כפולה לא מזהה שגיאה שפוגעת באותו מקום בשני העותקים (למשל קו תקוע או הפרעה מחזורית), ואם העותקים שונים לא יודעים איזה מהם נכון. ‏CRC מתוכנן מתמטית לזהות את כל שגיאות הביט הבודד ואת כל ה-bursts של עד 8 ביט, בעלות של בית אחד.
