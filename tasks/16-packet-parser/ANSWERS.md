# Task 16 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why must the parser check `LEN > PKT_MAX_PAYLOAD` before storing any payload byte? What would a corrupted LEN of 200 do otherwise?

Answer:
LEN must be checked to prevent writing outside the array. A LEN of 200 would overwrite other memory.

## Q2. Bytes are lost on the wire: the receiver misses the LEN byte of a frame. Walk through what your state machine does and how it gets back in sync.

Answer: 
The parser reads the first payload byte as LEN. It detects an error or a wrong checksum, then waits for the next START byte.


## Q3. Why does the parser keep a separate `done[]` copy of the last complete payload, instead of letting the caller read `payload[]` directly?

Answer: 
done[] keeps the last valid packet. payload[] may contain incomplete or invalid data.


## Q4. A XOR checksum is used here. What kind of error can it NOT detect, and when would you use the CRC-8 from task 13 instead?

Answer: 
XOR cannot detect some bit changes or swapped bytes. CRC-8 is better when more reliable error detection is needed.





