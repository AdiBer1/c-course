# Task 16 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why must the parser check `LEN > PKT_MAX_PAYLOAD` before storing any payload byte? What would a corrupted LEN of 200 do otherwise?

Answer:
LEN comes from the wire, so it can be corrupted. `payload[]` has only 32 bytes.
With LEN = 200 the parser would write 168 bytes past the end of the array and
overwrite `done_len`, `done[]`, `errors` and whatever is after the struct.
So I check LEN first and drop the frame.

## Q2. Bytes are lost on the wire: the receiver misses the LEN byte of a frame. Walk through what your state machine does and how it gets back in sync.

Answer:
After 0x7E the parser takes the first payload byte as LEN. Then it reads the
wrong number of bytes, so the checksum fails (or LEN > 32 fails). Either way
it returns an error and goes back to WAIT_START. There it ignores everything
until the next 0x7E, so the next frame is received correctly. Only one frame
is lost.

## Q3. Why does the parser keep a separate `done[]` copy of the last complete payload, instead of letting the caller read `payload[]` directly?

Answer:
`payload[]` is overwritten as soon as the next frame starts, and it may hold a
half-received or bad frame. `done[]` changes only on PKT_COMPLETE, so the
caller always reads a whole, checked packet, even if a bad frame arrives later.

## Q4. A XOR checksum is used here. What kind of error can it NOT detect, and when would you use the CRC-8 from task 13 instead?

Answer:
XOR cannot detect two errors in the same bit position (they cancel out), and
it cannot detect bytes that swap places, because XOR does not care about
order. CRC-8 catches these too. I would use CRC-8 on a noisy link or when a
wrong packet is dangerous, for example motor commands.