# Task 16 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why must the parser check `LEN > PKT_MAX_PAYLOAD` before storing any payload byte? What would a corrupted LEN of 200 do otherwise?

Answer:
LEN comes from the wire, so it can be anything: noise, a bit flip, or an attacker sending a crafted frame. The `payload` array has only 32 bytes. If LEN = 200 were accepted, the parser would store 200 bytes, so bytes 32..199 would be written past the end of `payload[]`, over `done_len`, `done[]`, `errors` and whatever lies after the struct in memory. That is a classic buffer overflow: it corrupts other variables, can crash the firmware, and an attacker who controls the bytes can overwrite data or even code pointers on purpose. Checking LEN in the LEN state, before entering the PAYLOAD state, means no payload byte is ever stored for an invalid frame, so the index can never go past 31.

## Q2. Bytes are lost on the wire: the receiver misses the LEN byte of a frame. Walk through what your state machine does and how it gets back in sync.

Answer:
The parser sees 0x7E and goes to the LEN state, but the next byte it receives is really the first payload byte, which it takes as LEN. If that value is greater than 32, it immediately reports ERR_LENGTH and goes back to WAIT_START. Otherwise it collects that many "payload" bytes (really payload, checksum, and maybe bytes from the next frame) and then compares a checksum that is almost certainly wrong, so it reports ERR_CHECKSUM and goes back to WAIT_START. Either way the broken frame is dropped and counted in `errors`, and the last good payload in `done[]` is untouched. In WAIT_START every byte except 0x7E is ignored, so the parser skips the rest of the broken frame and locks onto the next 0x7E start byte. At worst, the frame after the broken one is lost too, if its start byte was swallowed as payload. After that it is back in sync.

## Q3. Why does the parser keep a separate `done[]` copy of the last complete payload, instead of letting the caller read `payload[]` directly?

Answer:
`payload[]` is a work area: it is overwritten as soon as the next frame starts arriving, which can happen in an interrupt at any moment, and it may hold half of a frame or a frame that later fails its checksum. If the caller read `payload[]` directly, it could see a mix of the old and new frame, or the data of a broken frame. `done[]` is only written at one point, when a frame has fully arrived AND its checksum is correct. So it always holds one complete, verified packet, stays valid until the next PKT_COMPLETE, and a broken frame never destroys the last good one. This is double buffering: one buffer for receiving, one for the result.

## Q4. A XOR checksum is used here. What kind of error can it NOT detect, and when would you use the CRC-8 from task 13 instead?

Answer:
XOR works on each bit position separately, so it cannot detect any error that flips the same bit position an even number of times. For example, if bit 0 flips in two different bytes, both flips cancel out in the XOR and the checksum still matches. It also cannot detect swapped bytes, because XOR does not care about order: `01 02` and `02 01` give the same checksum. And it cannot detect a missing or extra 0x00 byte when the length still fits. A CRC-8 mixes every bit into the whole register through shifts, so it detects all of these, including all single- and double-bit errors and burst errors up to 8 bits. I would use CRC-8 whenever the data controls something important (motor commands, configuration, safety values) or the link is noisy (long cables, radio). XOR is acceptable only for short frames on a clean, short link where speed and simplicity matter most.