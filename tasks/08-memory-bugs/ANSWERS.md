# Task 08 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Before your fix, `log_add` wrote `values[8]` when the log was full, and most of the time nothing visibly broke. What memory did it overwrite, and why is "it seemed to work" so dangerous?

Answer:
`values[8]` נמצא מיד אחרי סוף המערך, בזיכרון הסטטי, ושם יושבים משתנים גלובליים אחרים של המודול (למשל `count` או `label`, לפי הסידור שהלינקר בחר). הכתיבה דרסה בשקט את אחד מהם. "זה עבד" רק כי במקרה מה שנדרס לא נבדק מיד, או שהבתים שם לא היו בשימוש. בסידור זיכרון אחר, בקומפיילר אחר או ברמת אופטימיזציה אחרת, אותו קוד ישבש משתנה חשוב ויקרוס או יתנהג מוזר במקום אחר לגמרי. ‏Undefined behaviour לא מבטיח קריסה, ולכן הבדיקות "עוברות" והבאג מגיע ללקוח.

## Q2. Explain the bug in `log_label` using the words stack, lifetime and dangling pointer. Why did making the buffer `static` fix it?

Answer:
`char buf[16]` היה משתנה מקומי, כלומר יושב על ה-**stack**, וה-**lifetime** שלו נגמר כשהפונקציה חוזרת. הפונקציה שמרה את הכתובת שלו ב-`label` והחזירה אותה, כך ש-`label` הפך ל-**dangling pointer**: מצביע לזיכרון שכבר לא שייך לאף אחד, וקריאה הבאה של פונקציה כלשהי תדרוס אותו. ‏`static char buf[16]` מאוחסן בזיכרון הסטטי, ה-lifetime שלו הוא כל זמן ריצת התוכנית, ולכן המצביע נשאר תקף ומצביע תמיד לאותו מקום, בדיוק כמו שה-header דורש.

## Q3. The average bug with `int16_t sum` is not a memory bug and no sanitizer reported it. How did you find it, and how did you choose the new type?

Answer:
הבדיקה של ממוצע עם ערכים גדולים נכשלה: סכום של כמה ערכים קרובים ל-32767 לא נכנס ב-`int16_t`, ולכן הוא "התגלגל" למספר שלילי והממוצע יצא שגוי. ‏(Sanitizer לא תפס את זה כי `sum += values[i]` מחושב ב-int ואז מומר ל-`int16_t`, וזו המרה ולא גלישה שמוגדרת כ-UB.) כדי לבחור טיפוס חישבנו את המקרה הגרוע: ‏8 ערכים כפול 32767 שווה 262136 בערך מוחלט, הרבה מעבר ל-16 ביט, אבל בתוך `int32_t` בקלות. לכן `int32_t`. באותה פונקציה תיקנו גם את החלוקה באפס כשהלוג ריק.

## Q4. Why does embedded code avoid `malloc`, and why does this log use a fixed static array instead?

Answer:
ב-embedded יש מעט RAM ואין מי שיטפל בכשל: `malloc` יכול להיכשל בזמן ריצה (בגלל זיכרון מלא או פרגמנטציה), זמן הריצה שלו לא צפוי, ומכשיר שרץ חודשים צובר פרגמנטציה. מערך סטטי מוקצה בזמן קומפילציה, הגודל שלו מופיע בדו"ח של הלינקר, ואם אין מספיק RAM הבנייה נכשלת מראש ולא אצל הלקוח. אין דליפות, אין free כפול, והזמן תמיד צפוי.
