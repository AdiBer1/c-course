# Task 17 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why does `q16_mul` need an `int64_t` intermediate, and why is the result of `a * b` then divided by 65536 (and not by 65536 * 65536)?

Answer:
שני מספרי Q16.16 מוכפלים נותנים מכפלה גדולה שעלולה להגיע ל-2^62, ולכן היא לא נכנסת ל-`int32`. המכפלה של שני ערכי raw מייצגת ערך בפורמט Q32.32, כלומר היא מכילה את הגורם 2^16 פעמיים. כדי לחזור ל-Q16.16 מחלקים פעם אחת ב-65536 בלבד.


## Q2. `x << 16` looks like the natural way to convert an integer to Q16.16. Why did you (or should you) avoid it for negative numbers and large values?

Answer:
הזזה שמאלה של מספר שלילי היא התנהגות לא מוגדרת לפי התקן, ולכן עדיף כפל. הזזה של מספר גדול גורמת לחיתוך שקט של הביטים העליונים, כלומר לתוצאה שגויה בלי שום אזהרה. לכן בודקים את הטווח ומחזירים ערך רווי.


## Q3. A PID controller computes `integral += error * ki` every millisecond. What happens with wrapping (non-saturating) arithmetic when the integral gets large, and why is saturation the safer behaviour for a motor?

Answer:
עם עטיפה (wrapping), כשהאינטגרל עובר את המקסימום הוא חוזר למינימום, והבקרה מקבלת פתאום פקודה הפוכה מהמצופה. זה יגרום לתנועה חדה או להיפוך כיוון של המנוע. עם חיתוך (saturation) האינטגרל נעצר בגבול, והמערכת יוצאת ממנו בצורה חלקה כשהשגיאה משתנה.


## Q4. Q16.16 has a resolution of about 0.000015 and a range of ±32768. For a battery voltage in volts (0–60 V) or a GPS coordinate in degrees (±180, needs 0.0000001), is Q16.16 a good choice? Explain.

Answer:
למתח הסוללה, Q16.16 מתאים מאוד: הטווח מכסה 0 עד 60 V בנוחות, והרזולוציה של כ-0.000015 V עדיפה בהרבה על הדרישה. לקואורדינטת GPS הוא לא מתאים, כי הרזולוציה של Q16.16 היא בערך 0.000015 מעלות, שזה כמה מטרים, ולא מגיעה ל-0.0000001 מעלות שנדרשת. לשם צריך פורמט עם יותר ביטים לשבר, או מספר שלם בסקלה של מיקרו-מעלות.

