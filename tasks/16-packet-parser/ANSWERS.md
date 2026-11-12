# Task 16 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why must the parser check `LEN > PKT_MAX_PAYLOAD` before storing any payload byte? What would a corrupted LEN of 200 do otherwise?

Answer:
It prevents a buffer overflow. Without this check, a length of 200 would write past the end of the array and corrupt adjacent memory.
## Q2. Bytes are lost on the wire: the receiver misses the LEN byte of a frame. Walk through what your state machine does and how it gets back in sync.

Answer:
The first payload byte is mistakenly read as LEN, which will either exceed the max length or cause a checksum mismatch. Either way, the state machine resets to WAIT_START and resynchronizes on the next start byte.
## Q3. Why does the parser keep a separate `done[]` copy of the last complete payload, instead of letting the caller read `payload[]` directly?

Answer:
payload[] acts as an active buffer for incoming data, while done[] holds the last verified packet. This keeps valid data safe from being overwritten by corrupted or incomplete frames.
## Q4. A XOR checksum is used here. What kind of error can it NOT detect, and when would you use the CRC-8 from task 13 instead?

Answer:
XOR misses bit flips that occur at the same bit position across an even number of bytes, as well as byte reordering. CRC-8 should be used over noisy channels where stronger integrity checks against multi-bit or burst errors are needed.
