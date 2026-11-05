# Task 16 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why must the parser check `LEN > PKT_MAX_PAYLOAD` before storing any payload byte? What would a corrupted LEN of 200 do otherwise?

Answer:
Checking `LEN > PKT_MAX_PAYLOAD` immediately prevents buffer overflow vulnerabilities. Because the internal `payload[]` buffer has a fixed capacity of 32 bytes, a corrupted `LEN` of 200 without validation would cause the parser to write 200 bytes out-of-bounds, leading to memory corruption and undefined behavior.

## Q2. Bytes are lost on the wire: the receiver misses the LEN byte of a frame. Walk through what your state machine does and how it gets back in sync.

Answer:
If the `LEN` byte is lost, the parser misinterprets the first byte of the payload as `LEN`. It processes the subsequent bytes accordingly until the checksum calculation almost certainly fails (or `LEN` exceeds 32), returning an error and resetting to `WAIT_START`. Once back in `WAIT_START`, it discards all incoming garbage bytes until it encounters the next valid `PKT_START` (`0x7E`), fully re-synchronizing with the stream.

## Q3. Why does the parser keep a separate `done[]` copy of the last complete payload, instead of letting the caller read `payload[]` directly?

Answer:
The `payload[]` array serves as an active workspace while a new frame is actively being received. If an incoming frame fails checksum validation or is interrupted mid-packet, `payload[]` contains partial or corrupted data. Copying validated data to `done[]` ensures that the caller always accesses a fully verified frame without risk of reading corrupted, incomplete payload data from a failed frame.

## Q4. A XOR checksum is used here. What kind of error can it NOT detect, and when would you use the CRC-8 from task 13 instead?

Answer:
A XOR checksum cannot detect an even number of bit inversions at the exact same bit position across different bytes, nor can it detect byte swapping/reordering (since XOR is commutative). CRC-8 (from task 13) should be used instead in noisier channels where burst errors, multi-bit corruptions, or byte reordering are more common and require stronger mathematical error-detection guarantees.

