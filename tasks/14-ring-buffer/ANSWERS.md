# Task 14 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. If you only had `head` and `tail` (no `count`), `head == tail` could mean "empty" or "full". How does the struct here solve that, and what is the other common solution?

Answer:
הסטרקט שומר `count` נפרד שמבדיל בין ריק (count=0) למלא (count=size). הפתרון האחר הנפוץ: שמור רק size-1 בתים ב-buffer — אז head==tail תמיד אומר ריק, ו-head+1==tail אומר מלא.

## Q2. Many implementations require the size to be a power of two and use `index & (size - 1)` instead of a comparison or `%`. Why is that popular on microcontrollers?

Answer:
`%` (modulo) מתורגם לחלוקה שהיא פעולה יקרה על MCU ללא חומרת חלוקה. `& (size-1)` הוא פעולת AND בודדת — מהיר פי עשרות.

## Q3. In a UART driver, the receive interrupt pushes and the main loop pops. Which fields does each side write? Why can this be dangerous with the `count` design, and what would you do about it?

Answer:
ISR כותב ל-`head` ו-`count`. main loop כותב ל-`tail` ו-`count`. שניהם כותבים ל-`count` — read-modify-write שאינו אטומי. פתרון: השתמש ב-critical section (disable interrupts) בעת עדכון `count`, או עבור לעיצוב ללא `count`.

## Q4. Why does the caller provide the storage array instead of the ring buffer having a fixed internal array?

Answer:
כך אפשר ליצור מספר ring buffers בגדלים שונים עם אותו קוד. כל buffer מקבל מערך סטטי משלו שהקורא מגדיר — גמישות מקסימלית ללא malloc.
