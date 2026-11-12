# Task 14 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. If you only had `head` and `tail` (no `count`), `head == tail` could mean "empty" or "full". How does the struct here solve that, and what is the other common solution?

Answer:
After `size` pushes into an empty buffer, `head` has gone all the way around and is back at `tail`, which is exactly what it looks like when the buffer is empty. The struct here adds a separate `count` field: empty is `count == 0` and full is `count == size`, so `head == tail` never has to be interpreted, and all `size` bytes can be used. The other common solution is to keep only head and tail and always leave one slot unused: the buffer is "full" when `next(head) == tail`, so `head == tail` can only mean empty. That avoids the extra field, but a buffer of `size` bytes can then hold only `size - 1` bytes.

## Q2. Many implementations require the size to be a power of two and use `index & (size - 1)` instead of a comparison or `%`. Why is that popular on microcontrollers?

Answer:
When `size` is a power of two, `size - 1` is a mask of all ones (for 64 it is 0b111111), so `index & (size - 1)` wraps the index around in a single AND instruction, with no branch. The `%` operator needs a division, and many small MCUs (Cortex-M0, AVR) have no hardware divide instruction, so `%` becomes a slow library call taking dozens of cycles. A comparison with a branch is cheaper than `%`, but still slower than one AND, and the time can vary. In code that runs inside an interrupt on every received byte, a fast and constant-time wrap matters. Some implementations even let the indices run freely and mask only when accessing the array, which also solves the full/empty problem.

## Q3. In a UART driver, the receive interrupt pushes and the main loop pops. Which fields does each side write? Why can this be dangerous with the `count` design, and what would you do about it?

Answer:
The interrupt (push) writes `buf[head]`, `head` and `count`. The main loop (pop) writes `tail` and `count`. `head` and `tail` each have only one writer, but `count` is written by BOTH sides. `count++` and `count--` are read-modify-write operations (load, change, store), not single instructions. If the interrupt fires in the middle of the main loop's `count--`, after the load but before the store, the main loop stores its old value and the interrupt's increment is lost. The count is then wrong forever, and bytes are lost or read twice. Fixes: disable the UART interrupt for the few instructions of `rb_pop` (a short critical section), or remove the shared field altogether by using the "one empty slot" design where the ISR only writes `head` and the main loop only writes `tail`, so each index has a single writer. On a multi-core or optimizing setup the indices must also be `volatile` or atomic.

## Q4. Why does the caller provide the storage array instead of the ring buffer having a fixed internal array?

Answer:
Different buffers need different sizes: a UART receive buffer may need 64 bytes, a log queue 512, an audio stream 2 KB. With caller-provided storage, one implementation serves all of them, and each user picks exactly the size it needs. A fixed internal array would force one size on everyone, wasting RAM in some places and being too small in others. It also fits the no-malloc rule: the caller declares `static uint8_t rx_storage[64];`, so all memory is static, visible in the linker's RAM report, and known at compile time. The caller can also place the storage where it wants, for example in a special RAM section that DMA can access.