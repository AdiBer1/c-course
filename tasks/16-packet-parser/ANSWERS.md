# Task 16 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why must the parser check `LEN > PKT_MAX_PAYLOAD` before storing any payload byte? What would a corrupted LEN of 200 do otherwise?

Answer:
Checking `LEN` immediately in the `PKT_STATE_LEN` state prevents incoming data from overflowing the fixed `payload` buffer of size `PKT_MAX_PAYLOAD` (32 bytes). If a corrupted `LEN` of 200 were accepted, the parser would attempt to write 200 bytes into a 32-byte array, causing a severe buffer overflow and memory corruption on adjacent stack/struct fields.

## Q2. Bytes are lost on the wire: the receiver misses the LEN byte of a frame. Walk through what your state machine does and how it gets back in sync.

Answer:
If the `LEN` byte is lost, the parser interprets the first payload byte as `LEN`. If this byte exceeds 32, it immediately drops the frame with `PKT_ERR_LENGTH` and resets to `WAIT_START`. If it is $\le 32$, the parser reads payload and checksum bytes; the checksum will almost certainly fail, causing a `PKT_ERR_CHECKSUM` drop and resetting to `WAIT_START`. In both cases, the parser safely discards the bad frame and re-synchronizes as soon as it sees the next `0x7E` start byte.

## Q3. Why does the parser keep a separate `done[]` copy of the last complete payload, instead of letting the caller read `payload[]` directly?

Answer:
`payload[]` is an active working buffer that gets overwritten as new incoming bytes arrive. If the caller read directly from `payload[]`, an incoming partial or invalid frame would corrupt the data being read by the application. Copying to `done[]` only on `PKT_COMPLETE` ensures that the application always has a stable, uncorrupted copy of the last successfully validated packet.

## Q4. A XOR checksum is used here. What kind of error can it NOT detect, and when would you use the CRC-8 from task 13 instead?

Answer:
A simple XOR checksum cannot detect multi-bit errors that preserve bit parity, such as swapping the order of two bytes or two identical bit-flips in the same bit position across different bytes. You would use CRC-8 when operating over noisy serial channels (like UART or radio links) where burst errors and byte-reorderings occur, as polynomial CRC division catches these multi-bit error patterns with much higher reliability.

