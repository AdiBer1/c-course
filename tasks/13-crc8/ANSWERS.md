# Task 13 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simple checksum adds all bytes. Give an error that a sum does not detect but a CRC does.

Answer:
החלפת שני בתים זה בזה (swap): סכום לא ישתנה כי חיבור הוא קומוטטיבי. CRC מזהה זאת כי הוא תלוי בסדר הבתים.

## Q2. Inside `crc8_update`, `crc << 1` on a `uint8_t` produced a `-Wconversion` warning until you cast. What is the type of `crc << 1`, and why is the cast correct here rather than a hack?

Answer:
`crc << 1` הוא `int` (integer promotion). ה-cast ל-`uint8_t` נכון כי אנחנו רוצים לשמור רק 8 ביטים — זה בדיוק מה שאלגוריתם CRC-8 דורש.

## Q3. The bit-by-bit CRC needs 8 loop iterations per byte. A table-driven version uses a 256-byte lookup table and one step per byte. When would you choose each on a microcontroller?

Answer:
Bit-by-bit: כשה-flash קטן ומהירות לא קריטית. Table-driven: כשצריך מהירות גבוהה ויש 256 בתים פנויים ב-flash. על MCU עם 2KB flash — bit-by-bit. על MCU עם 64KB flash — table.

## Q4. Why does the receiver compute the CRC over the data and compare, instead of the sender just sending the data twice?

Answer:
שליחה פעמיים מכפילה את הנתונים אבל לא מגנה מפני שגיאות שמשפיעות על שתי העותקים באותו אופן. CRC מזהה שגיאות ביעילות עם בית אחד בלבד, ומגן מפני סוגי שגיאות שכפול לא יזהה.
