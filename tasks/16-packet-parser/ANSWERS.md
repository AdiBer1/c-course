# Task 16 — Answers

## Q1. Why must the parser check `LEN > PKT_MAX_PAYLOAD` before storing any payload byte? What would a corrupted LEN of 200 do otherwise?

Answer: 
The parser must validate `LEN` immediately to prevent buffer overflows, since the internal `payload[]` array is statically allocated to a maximum size (`PKT_MAX_PAYLOAD`). A corrupted `LEN` of 200 would cause an out-of-bounds write past the allocated buffer, corrupting adjacent stack or struct memory and leading to undefined behavior or security vulnerabilities.

## Q2. Bytes are lost on the wire: the receiver misses the LEN byte of a frame. Walk through what your state machine does and how it gets back in sync.

Answer: 
If the `LEN` byte is missed, the receiver treats the next incoming byte (which is actually part of the payload) as the `LEN`. If that byte happens to be unrealistically large (`> PKT_MAX_PAYLOAD`), the parser triggers `PKT_ERR_LENGTH` and resets immediately to `PKT_STATE_WAIT_START`. If it happens to be a valid small number, the parser will misinterpret subsequent payload bytes as the rest of the frame, leading to a checksum mismatch (`PKT_ERR_CHECKSUM`), which then causes it to drop the frame and return safely to `PKT_STATE_WAIT_START` to catch the next start byte (`0x7E`).

## Q3. Why does the parser keep a separate `done[]` copy of the last complete payload, instead of letting the caller read `payload[]` directly?

Answer: 
The parser builds incoming packets into a working buffer (`payload[]`) byte by byte. If a current frame fails partway through or fails its checksum, keeping a separate `done[]` buffer ensures that the last successfully received and verified payload remains intact and undisturbed for the caller until a new valid packet arrives.

## Q4. A XOR checksum is used here. What kind of error can it NOT detect, and when would you use the CRC-8 from task 13 instead?

Answer: 
A XOR checksum cannot detect errors where an even number of identical bits are flipped in the same positions across bytes (as their XOR contributions cancel out), nor can it detect transposition errors (reordering of bytes). You would use the CRC-8 from task 13 instead when dealing with noisier communication channels where robust error detection against burst errors or bit permutations is required.