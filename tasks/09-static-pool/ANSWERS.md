# Task 09 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Where in memory does your pool's storage live (stack, static, heap)? When is that memory reserved, and what does that mean for the linker's RAM report?

Answer:
המערך `storage` (ו-`in_use`) מוגדרים `static` ברמת הקובץ, ולכן הם בזיכרון הסטטי (סגמנט `.bss`, כי אין להם ערך התחלתי), לא על ה-stack ולא ב-heap. הזיכרון נשמר בזמן הלינקוק, וקיים כל זמן ריצת התוכנית. לכן 8 × 32 = 256 הבתים מופיעים בדו"ח ה-RAM של הלינקר, ואם אין מספיק RAM זה מתגלה בזמן הבנייה ולא באמצע הריצה.

## Q2. Why must a pool block be aligned to `_Alignof(max_align_t)`? What could happen on an ARM Cortex-M0 if someone stored a `uint32_t` at an odd address?

Answer:
הבלוק מוחזר כ-`void *`, והקורא יכול לשים בו כל טיפוס: `uint32_t`, `double`, struct. כדי שזה יהיה חוקי בכל מקרה, הכתובת צריכה להיות מיושרת לטיפוס הכי תובעני, וזה `max_align_t`. בקוד הגדרתי את המערך עם `_Alignas(max_align_t)`, וגודל בלוק 32 הוא כפולה של היישור, כך שכל בלוק מיושר. ‏Cortex-M0 לא תומך בגישה לא מיושרת: `LDR`/`STR` של 32 ביט לכתובת שלא מתחלקת ב-4 גורם ל-HardFault, והמכשיר נתקע או מתאפס.

## Q3. With `malloc`, a long-running device can fail to allocate even though enough bytes are free in total. Explain why, and why a pool of equal-size blocks cannot have this problem.

Answer:
זו פרגמנטציה: אחרי הרבה הקצאות ושחרורים בגדלים שונים, ה-heap מתפצל לחורים קטנים בין בלוקים תפוסים. יכולים להיות למשל 200 בתים פנויים בסך הכל, אבל אף חור לא גדול מ-40, ואז בקשה של 100 בתים נכשלת. ב-pool כל הבלוקים באותו גודל וכל בקשה היא בדיוק בלוק אחד, כך שכל בלוק פנוי מתאים לכל בקשה. אם `pool_free_count() > 0` ההקצאה תמיד מצליחה.

## Q4. How does your `pool_free` detect a double free and a pointer that is not the start of a block? Why is it better to detect these than to silently accept them?

Answer:
הפונקציה ממירה את המצביע ל-`uintptr_t` ובודקת שהוא בתוך טווח המערך (`start <= addr < start + 8*32`). אחר כך מחשבת `offset = addr - start` ובודקת `offset % POOL_BLOCK_SIZE == 0`: אם לא, המצביע באמצע בלוק. ‏`offset / POOL_BLOCK_SIZE` נותן את מספר הבלוק, ואם `in_use[index]` כבר false זה double free. בכל המקרים האלה מחזירה false בלי לשנות כלום. אם היינו מקבלים אותם בשקט, אותו בלוק היה יכול להיחשב פנוי פעמיים ולהימסר לשני משתמשים, שידרסו זה את הנתונים של זה. זה באג שקשה מאוד לאתר, ובדיקה מוקדמת חושפת אותו מיד במקום שבו הוא קרה.
