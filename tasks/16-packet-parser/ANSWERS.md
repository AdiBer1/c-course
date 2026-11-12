# Task 16 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why must the parser check `LEN > PKT_MAX_PAYLOAD` before storing any payload byte? What would a corrupted LEN of 200 do otherwise?

Answer:
The internal `payload` buffer has a fixed static size of `PKT_MAX_PAYLOAD` (32 bytes). If the parser did not check `LEN` first, a corrupted length of 200 would cause a buffer overflow, writing 168 bytes past the end of `payload[]` and corrupting adjacent struct fields (`done_len`, `done`, `errors`) or memory.

## Q2. Bytes are lost on the wire: the receiver misses the LEN byte of a frame. Walk through what your state machine does and how it gets back in sync.

Answer:
After receiving `0x7E`, the parser transitions to `PKT_STATE_LEN` and mistakenly reads the first payload byte as `LEN`. If that byte is greater than 32, it immediately drops the frame with `PKT_ERR_LENGTH` and returns to `PKT_STATE_WAIT_START`. If it is 32 or less, it reads that many subsequent bytes as payload and checks the next byte as the checksum, which will almost certainly fail (`PKT_ERR_CHECKSUM`) and reset the parser back to `PKT_STATE_WAIT_START` to resynchronize on the next `0x7E` start byte.

## Q3. Why does the parser keep a separate `done[]` copy of the last complete payload, instead of letting the caller read `payload[]` directly?

Answer:
Because `payload[]` is actively overwritten byte-by-byte as soon as the next incoming packet begins arriving. Keeping a separate `done[]` buffer guarantees that the last valid packet remains intact and safe for the caller to read at any time, even while a new frame is mid-reception or if a corrupted frame is dropped.

## Q4. A XOR checksum is used here. What kind of error can it NOT detect, and when would you use the CRC-8 from task 13 instead?

Answer:
A simple XOR checksum is commutative and linear, so it cannot detect reordered bytes (transposition errors) or an even number of bit flips occurring at the same bit position across different bytes. You would use CRC-8 when communicating over noisy serial links where burst errors, bit-order swaps, or multiple bit flips are likely and stronger error detection is required.
