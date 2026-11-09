# Task 17 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why does `q16_mul` need an `int64_t` intermediate, and why is the result of `a * b` then divided by 65536 (and not by 65536 * 65536)?

Answer:
שני מספרי Q16.16 הם `int32_t`. כפל שלהם יכול להגיע עד 2^62 — גדול מדי ל-`int32_t` (overflow). `int64_t` מחזיק עד 2^63 ולכן מספיק. מחלקים ב-65536 (ולא ב-65536²) כי כל אחד מהאופרנדים כבר מוכפל ב-65536 — המכפלה מוכפלת ב-65536² ולכן מחלקים פעם אחת ב-65536 כדי לחזור לפורמט Q16.16.

## Q2. `x << 16` looks like the natural way to convert an integer to Q16.16. Why did you (or should you) avoid it for negative numbers and large values?

Answer:
הזזה שמאלה של מספר שלילי היא undefined behavior ב-C. בנוסף, עבור ערכים גדולים (מעל 32767) ה-shift יגרום לoverflow ב-`int32_t`. במקום זאת, כופלים ב-65536 עם `int64_t` ואז בודקים saturation — זה מוגדר ובטוח.

## Q3. A PID controller computes `integral += error * ki` every millisecond. What happens with wrapping (non-saturating) arithmetic when the integral gets large, and why is saturation the safer behaviour for a motor?

Answer:
עם wrap-around, האינטגרל יכול לעבור מהערך המקסימלי לשלילי גדול בפתאומיות — המנוע יקבל פקודה הפוכה ויזנק לכיוון ההפוך. זה מסוכן פיזית. עם saturation, האינטגרל נשאר בערך המקסימלי — המנוע ממשיך לדחוף בכיוון הנכון ולא קופץ.

## Q4. Q16.16 has a resolution of about 0.000015 and a range of ±32768. For a battery voltage in volts (0–60 V) or a GPS coordinate in degrees (±180, needs 0.0000001), is Q16.16 a good choice? Explain.

Answer:
מתח סוללה (0–60V): Q16.16 מתאים — הטווח ±32768 מכסה 60V, והרזולוציה 0.000015V מספיקה לכל מדידת מתח. קואורדינטת GPS: לא מתאים — נדרשת רזולוציה של 0.0000001 אבל Q16.16 נותן רק 0.000015, פי 150 גרוע מדי. לGPS צריך double או פורמט Q כבד יותר.
