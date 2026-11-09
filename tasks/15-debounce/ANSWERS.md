# Task 15 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simpler debounce is: "when the input changes, wait 20 ms with a delay loop, then read it again". Why is that a bad idea in firmware?

Answer:
delay loop חוסם את ה-CPU לכל אותו זמן. שאר הקוד (UART, תצוגה, state machines אחרים) לא יכול לרוץ. ב-firmware non-blocking חייבים לחזור מיד ולתת ל-main loop להמשיך.

## Q2. Draw (in words) the states and transitions of your debouncer. What exactly makes it ignore a 3-tick glitch when the threshold is 5?

Answer:
מצב: stable=RELEASED, counter=0. כל tick שהקלט שונה מהמצב היציב — counter++. אם counter < threshold — לא קורה כלום. glitch של 3 ticks: counter מגיע ל-3 ואז הקלט חוזר — counter מתאפס. לעולם לא מגיע ל-5.

## Q3. Why is all the debouncer's state inside a `debounce_t` struct passed by pointer, instead of `static` variables in debounce.c?

Answer:
כך אפשר להשתמש בכמה כפתורים במקביל — כל אחד עם `debounce_t` משלו. עם static variables היה אפשר לנהל רק כפתור אחד.

## Q4. With a 1 ms tick and threshold 5, what is the delay between the contact settling and the PRESS event? How would you choose the threshold for a real button?

Answer:
5 ms. לכפתור אמיתי בוחרים threshold לפי זמן ה-bounce של הכפתור הספציפי (בדרך כלל 5-20 ms). threshold = זמן_bounce / זמן_tick.
