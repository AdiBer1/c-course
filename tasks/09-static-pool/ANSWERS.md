# Task 09 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Where in memory does your pool's storage live (stack, static, heap)? When is that memory reserved, and what does that mean for the linker's RAM report?

Answer:
ב-static memory (BSS/data segment). הזיכרון שמור בזמן קומפילציה/קישור. הלינקר יכלול אותו בדוח ה-RAM, כך שאפשר לדעת בדיוק כמה RAM ישתמש הפרויקט לפני הרצה.

## Q2. Why must a pool block be aligned to `_Alignof(max_align_t)`? What could happen on an ARM Cortex-M0 if someone stored a `uint32_t` at an odd address?

Answer:
Cortex-M0 לא תומך בגישה לא מיושרת לזיכרון — קריאה/כתיבה של `uint32_t` בכתובת לא מיושרת גורמת ל-HardFault (קריסה). `max_align_t` מבטיח שהבלוק מיושר לכל סוג.

## Q3. With `malloc`, a long-running device can fail to allocate even though enough bytes are free in total. Explain why, and why a pool of equal-size blocks cannot have this problem.

Answer:
`malloc` סובל מפרגמנטציה: הזיכרון הפנוי מפוצל לחתיכות קטנות שאינן רצופות. Pool עם בלוקים שווים אינו סובל מזה — כל בלוק פנוי מתאים לכל בקשה, ואין פרגמנטציה.

## Q4. How does your `pool_free` detect a double free and a pointer that is not the start of a block? Why is it better to detect these than to silently accept them?

Answer:
עוברים על כל הבלוקים ומחפשים כתובת תואמת. אם נמצאה אבל `free_map[i]` כבר true — זה double free. אם לא נמצאה בכלל — מצביע לא תקף. זיהוי מוקדם מונע שחיתות זיכרון שקשה לאתר מאוחר יותר.
