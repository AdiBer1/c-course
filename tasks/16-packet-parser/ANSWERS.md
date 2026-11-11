# Task 16: Packet Parser - Answers

1. **Why is byte-by-byte state machine parsing suitable for embedded serial streams?**
   It allows stream processing without allocating large buffers or blocking execution, handling non-contiguous input gracefully.

2. **How does framing with start bytes, length fields, and checksums ensure data integrity?**
   Start bytes synchronization identifies frame boundaries, length fields define expected boundaries, and checksums detect corruption.

3. **What happens when a corrupt byte or invalid length is encountered?**
   The parser drops the frame, increments the error counter, and immediately resets to `WAIT_START` to resynchronize on the next valid packet.