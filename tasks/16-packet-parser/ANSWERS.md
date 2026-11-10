# Task 16 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why must the parser check `LEN > PKT_MAX_PAYLOAD` before storing any payload byte? What would a corrupted LEN of 200 do otherwise?

Answer:
The payload array is only 32 bytes long. Without checking LEN first, a value like LEN = 200 would cause the parser to write 200 bytes into it, overflowing the buffer and overwriting the fields right after it in the struct (done, errors, etc.) as well as memory beyond the struct. Since LEN comes straight off the wire, it can easily be corrupted or malicious, so we can't trust it.

## Q2. Bytes are lost on the wire: the receiver misses the LEN byte of a frame. Walk through what your state machine does and how it gets back in sync.

Answer:
If the LEN byte is lost, the parser sees 0x7E and treats the very next byte—which is actually the first byte of the data—as LEN. From there, it reads the wrong number of bytes, so the checksum will almost certainly fail and return PKT_ERR_CHECKSUM (or PKT_ERR_LENGTH if that misread byte happens to be greater than 32). Either way, the parser resets back to WAIT_START, ignores any incoming bytes until the next 0x7E, and resynchronizes from there. That broken packet is lost, and if the misread LEN is larger than the real one, the parser may also swallow the start of the next packet, so it can be lost too. But because every error returns to WAIT_START, the parser always resynchronizes within a packet or two.

## Q3. Why does the parser keep a separate `done[]` copy of the last complete payload, instead of letting the caller read `payload[]` directly?

Answer:
Q3:payload[] is a working buffer that gets filled as bytes arrive. If the caller read from it directly, it could end up reading a half-finished packet or one that later fails validation. done[] is only updated once a packet passes all checks, so it always holds a complete, valid packet, and a corrupted frame won't wipe out the last good one (which test_failed_frame_keeps_previous_payload checks).

## Q4. A XOR checksum is used here. What kind of error can it NOT detect, and when would you use the CRC-8 from task 13 instead?

Answer:
XOR misses two errors in the same bit position across two different bytes because the flips cancel each other out. It also can't detect swapped bytes since XOR is order-independent—A followed by B gives the exact same result as B followed by A. CRC-8 catches both byte reordering and burst errors (a string of consecutive corrupted bits), so you should use it when the line is noisy or when the data is critical, like motor commands.

