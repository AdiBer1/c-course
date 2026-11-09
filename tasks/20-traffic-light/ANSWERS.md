# Task 20 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `if (now >= tl->phase_start_ms + TL_RED_MS)` wrong near the 32-bit wrap-around, while `if ((uint32_t)(now - start) >= TL_RED_MS)` is right? Use concrete numbers.

Answer:
דוגמה: `start = 0xFFFFF000`, `TL_RED_MS = 5000`. אז `start + 5000 = 0x000013B8` (wrap). עכשיו `now = 0xFFFFF500` — עדיין לפני הסיום, אבל `now >= 0x000013B8` נכון (כי 0xFFFFF500 > 0x000013B8) — שגיאה. עם `(uint32_t)(now - start)`: `0xFFFFF500 - 0xFFFFF000 = 0x500 = 1280` — פחות מ-5000, נכון. חיסור unsigned תמיד עובד נכון עם wrap.

## Q2. What would happen to the rest of the firmware if `tl_update` used a delay loop to wait for the 5 seconds of RED? How does the test catch it?

Answer:
כל הקוד האחר בלולאה הראשית (UART, חיישנים, כפתורים) לא יריץ 5 שניות. הפירמוור "קפוא". הטסט מגביל כל קריאה ל-`tl_update` לזמן קצר — אם הפונקציה חוזרת רק אחרי 5 שניות, הטסט נכשל (timeout).

## Q3. When an update comes 300 ms late, your new phase starts at "now", not at "start + duration". What is the difference over many cycles, and when would you prefer the other choice?

Answer:
עם `phase_start = now`: כל עיכוב מצטבר — אחרי 100 מחזורים, הרמזור יכול להיות דקות מאחור. עם `phase_start = start + duration`: העיכוב לא מצטבר, הזמן הכולל נשמר מדויק. לרמזור ציבורי עדיף `start + duration`. לפירמוור פשוט, `now` מספיק ופשוט יותר לממש.

## Q4. This module only uses `hal_gpio_t` and `hal_clock_t`. List exactly what changes when you move it to a real microcontroller.

Answer:
רק שני דברים משתנים: (1) מימוש `hal_gpio_t` — פונקציות שכותבות לרגיסטרי GPIO של השבב הספציפי. (2) מימוש `hal_clock_t` — פונקציה שקוראת טיימר חומרה (כמו SysTick ב-ARM). הקוד של `traffic_light.c` עצמו לא משתנה כלל.
