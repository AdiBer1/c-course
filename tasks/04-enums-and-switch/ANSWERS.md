# Task 04 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `command_t` an `enum` and not just `int` constants like `#define CMD_LED_ON 1`? Name two concrete advantages.

Answer:
1. הקומפיילר יכול להזהיר על `switch` שלא מכסה ערך enum מסוים. 2. הקוד יותר קריא ובטוח טיפוסית כי הקומפיילר בודק שהערך הוא מהטיפוס `command_t` ולא סתם `int`.

## Q2. What does `-Wall` warn about if your `switch (cmd)` in `led_next_state` has no `default` and you forget one enum value? Why is that warning valuable when a new command is added later?

Answer:
`-Wall` מזהיר על `enumeration value not handled in switch`. זה יקר כי אם מוסיפים פקודה חדשה ל-enum בעתיד, הקומפיילר יזכיר לעדכן את כל ה-switch שקשורים אליה.

## Q3. A corrupted byte in RAM (or a bug elsewhere) can leave a `command_t` variable holding 42. What does your code do with it, and why is it important that firmware handles "impossible" values defensively?

Answer:
ה-`default` ב-switch מטפל בערך לא חוקי. ב-`command_name` הוא מחזיר "INVALID", ב-`led_next_state` הוא שומר על המצב הנוכחי. ב-firmware חשוב לטפל בערכים בלתי חוקיים כי זיכרון עלול להיפשע ולגרום להתנהגות בלתי צפויה.

