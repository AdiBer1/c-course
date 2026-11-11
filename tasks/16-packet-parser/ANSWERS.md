# Task 16 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why must the parser check `LEN > PKT_MAX_PAYLOAD` before storing any payload byte? What would a corrupted LEN of 200 do otherwise?

Answer:
Because if the value of LEN is bigger than the space allocated for the payload, we would store values in places that were not allocated for them and overwrite whatever is there in memory. This is a serious overflow. It could overwrite `done`, `errors`, etc., and even other things that are not related to this struct at all.

## Q2. Bytes are lost on the wire: the receiver misses the LEN byte of a frame. Walk through what your state machine does and how it gets back in sync.

Answer:
The first payload byte that arrives is taken as the LEN. Then either it is bigger than the allocated space, and we immediately return an error and go back to the initial state, or the machine thinks it is the length and continues receiving bytes for the payload, but the count is shifted by one the whole time. How do we discover the mistake? Because of the XOR check, the value will simply not be what was expected, and we will see that there was an error, and then we go back to the initial state again.

## Q3. Why does the parser keep a separate `done[]` copy of the last complete payload, instead of letting the caller read `payload[]` directly?

Answer:
So that if the message the parser is currently reading turns out to be invalid, it won't overwrite the last valid message.

## Q4. A XOR checksum is used here. What kind of error can it NOT detect, and when would you use the CRC-8 from task 13 instead?

Answer:
If there are 2 errors that cancel each other out, XOR will not reveal it. In addition, if bytes arrive in the wrong order, this also cannot be detected. It is better to use CRC when the data is important, or when there is significant noise on the line.