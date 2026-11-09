# Task 12 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `return *(const uint32_t *)p;` a bad way to implement `rd_u32_le`, even if it passes the tests on your PC? Give two separate reasons.

Answer:
1. Alignment: `p` עשוי להיות לא מיושר ל-4 בתים — על ARM Cortex-M0 זה גורם ל-HardFault. 2. Endianness: הקוד מניח שה-CPU הוא little-endian, אבל על big-endian CPU התוצאה תהיה הפוכה.

## Q2. `p[3] << 24` where `p[3]` is a `uint8_t` 0xFF: what is the type of the expression, and what did UBSan report if you didn't cast? How does the cast fix it?

Answer:
`p[3]` עובר integer promotion ל-`int` (signed). `0xFF << 24` = 0xFF000000 שמציף `int` — undefined behaviour. Cast ל-`(uint32_t)p[3]` לפני ההזזה פותר את זה כי unsigned overflow מוגדר.

## Q3. A temperature sensor sends its value big-endian as bytes `FF 38`. Walk through how you get -200 from those two bytes.

Answer:
`rd_u16_be`: `(0xFF << 8) | 0x38` = `0xFF38` = 65336 כ-uint16_t. Cast ל-`int16_t`: 0xFF38 = -200 ב-two's complement.

## Q4. Your firmware runs on a little-endian MCU and talks to a big-endian protocol. Why is it still better to call `rd_u32_be()` everywhere than to write "swap the bytes because we are little-endian"?

Answer:
אם יום אחד הקוד יועבר ל-big-endian MCU, הקוד עם `rd_u32_be()` ימשיך לעבוד נכון. קוד שמניח "אנחנו little-endian ולכן נהפוך" יישבר על big-endian CPU.
