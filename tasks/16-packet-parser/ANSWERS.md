# Task 16 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why must the parser check `LEN > PKT_MAX_PAYLOAD` before storing any payload byte? What would a corrupted LEN of 200 do otherwise?

Answer:
Because LEN comes from the wire and can be garbage: with LEN = 200 the parser would write 200 bytes into a 32-byte array and overwrite done[], the error counter and other memory after the struct.

## Q2. Bytes are lost on the wire: the receiver misses the LEN byte of a frame. Walk through what your state machine does and how it gets back in sync.

Answer:
The parser takes the next byte as LEN, reads the wrong number of bytes, and the checksum almost surely won’t match, so it reports an error, goes back to WAIT_START and ignores bytes until the next 0x7E.

## Q3. Why does the parser keep a separate `done[]` copy of the last complete payload, instead of letting the caller read `payload[]` directly?

Answer:
Because payload[] is overwritten by every new frame, even a broken one, while done[] only changes when a frame is complete and correct, so the caller always sees a whole, checked message.

## Q4. A XOR checksum is used here. What kind of error can it NOT detect, and when would you use the CRC-8 from task 13 instead?

Answer:
XOR can’t catch two flips of the same bit in two different bytes, or bytes that swapped places; CRC-8 also checks order and position, so I’d use it on noisy links or for important data like motor commands.q
