# Task 16 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why must the parser check `LEN > PKT_MAX_PAYLOAD` before storing any payload byte? What would a corrupted LEN of 200 do otherwise?

Answer:
The `payload` array has only 32 places. LEN comes from the wire, so it can be wrong because of noise, or someone can send a bad value on purpose. If LEN is 200 and we don't check it, we write 200 bytes into an array of 32, so we write over the memory after it: `done`, `errors`, and maybe other variables in the program. This is a buffer overflow. The program can crash or act strange, and it is also a security hole. So we check LEN first and drop the frame if it is too big.

## Q2. Bytes are lost on the wire: the receiver misses the LEN byte of a frame. Walk through what your state machine does and how it gets back in sync.

Answer:
The parser gets 0x7E and goes to the LEN state. The real LEN was lost, so the next byte it gets is the first payload byte, and it thinks this is LEN. If this value is bigger than 32, it returns ERR_LENGTH and goes back to WAIT_START. If not, it reads that many bytes as payload (wrong bytes), and then the checksum will almost always not match, so it returns ERR_CHECKSUM and goes back to WAIT_START. In WAIT_START it ignores everything until the next 0x7E, and from there the next frame is read normally. Maybe one more frame is lost on the way (if the wrong LEN "eats" its start byte), but after that it is in sync again.

## Q3. Why does the parser keep a separate `done[]` copy of the last complete payload, instead of letting the caller read `payload[]` directly?

Answer:
`payload[]` is the working area. It changes as soon as the next frame starts to arrive, and bytes can come from an interrupt while the main loop is still reading. If the next frame has a bad checksum, `payload[]` stays with half garbage. `done[]` is changed only when a frame is fully received and the checksum is OK. So the caller always gets a full and correct packet, and a bad frame can't destroy the last good one.

## Q4. A XOR checksum is used here. What kind of error can it NOT detect, and when would you use the CRC-8 from task 13 instead?

Answer:
XOR doesn't care about order, so if two bytes change places it gives the same result. Also if the same bit flips in two different bytes, the two errors cancel each other and the checksum still matches. A CRC catches these errors. I would use CRC-8 when the line is noisy, when the packets are longer, or when the data is important, like commands to a motor. XOR is OK only when the link is quite reliable and the CPU is very weak, because it is the fastest to compute.