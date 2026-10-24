# C for embedded systems: a self-paced course

You already know how to program. Here you learn **C**, the way it is used on
microcontrollers: fixed-width integers, pointers and memory you manage
yourself, bits and registers, state machines, and code that is separated from
the hardware so it can be tested on a PC. Everything runs on your computer; no
board is needed.

There are no lectures. For each task you:

1. read the task's `README.md` and its header file
2. write the code, with an **AI tutor** in the VS Code side chat that helps
   but never solves it for you
3. check yourself with `make score`
4. open a **pull request**: CI builds, tests and scores it (0–100) and
   posts the score as a comment
5. at **60 or more**, book a short **oral defense** with the teacher

---

## 1. One-time setup (Windows + WSL)

### 1.1 WSL and Ubuntu

In **PowerShell as Administrator**:

```powershell
wsl --install -d Ubuntu
```

Restart when asked, open "Ubuntu" from the Start menu and create your Linux
user name and password.

### 1.2 Tools inside Ubuntu

In the Ubuntu terminal:

```bash
sudo apt update
sudo apt install -y build-essential cppcheck git
sudo apt install -y clang-format valgrind     # optional: make format / make memcheck
gcc --version && make --version && cppcheck --version
git config --global user.name  "Your Name"
git config --global user.email "you@example.com"
```

### 1.3 VS Code

1. Install [VS Code](https://code.visualstudio.com/) **on Windows**.
2. Install the extensions **WSL** (Microsoft) and **C/C++** (Microsoft).
3. Choose an AI assistant for the side chat. Any assistant that runs in the
   VS Code chat works. One free option is **GitHub Copilot Free** (sign in with
   your GitHub account in the Copilot chat panel). See [§5](#5-the-ai-tutor).

### 1.4 Fork and clone

1. On GitHub, open <https://github.com/adiber1/c-course> and press **Fork**
   (keep the name `c-course`).
2. In the **Ubuntu** terminal (work inside Linux, not under `/mnt/c`, because
   it is much faster):

```bash
cd ~
git clone https://github.com/<your-user>/c-course.git
cd c-course
git remote add upstream https://github.com/adiber1/c-course.git
code .
```

`code .` opens VS Code **connected to WSL** (bottom-left corner shows
"WSL: Ubuntu"). Always open the course this way.

Check that everything works:

```bash
cd tasks/00-hello-toolchain
make test     # builds and runs the tests: they fail, that is expected
make score    # shows the score report
```

---

## 2. Working on a task

Example for task 07. Replace `07` and the folder name with your task.

```bash
cd ~/c-course

# 1. Start a branch for the task from the latest version of the course
git fetch upstream
git switch -c task-07 upstream/main

# 2. Work: edit only the files the task's README lists
cd tasks/07-command-parser
make test        # as often as you like
make score       # what CI will say; aim for 60+ before opening the PR

# 3. Commit (small commits are good: the teacher looks at your history)
git add src/parser.c ANSWERS.md
git commit -m "task 07: parse key and value"

# 4. Push the branch to YOUR fork
git push -u origin task-07
```

### Open the pull request

1. On GitHub, open your fork. A banner offers **Compare & pull request**;
   click it. (Or: **Pull requests → New pull request**.)
2. Check the direction: **base repository `adiber1/c-course`, base `main`**
   ← **head repository `<your-user>/c-course`, compare `task-07`**.
3. **Title:** exactly `[Task 07] - Full Name` (two-digit task number, then
   your full name), e.g. `[Task 07] - Dana Cohen`. A check called `title`
   turns red if the format is wrong; fix the title and it re-runs by itself.
4. Fill in the template (name, task, checklist, the three reflection
   questions) and press **Create pull request**.

The repository is **public**: never write your student ID, phone number or
other personal details in a PR.

The first time, the teacher approves your PR's CI run once (a GitHub rule for
new contributors). After that it runs by itself.

### Update the pull request

Keep working on the same branch and push again:

```bash
git add src/parser.c
git commit -m "task 07: handle range errors"
git push
```

CI runs again and the **same** score comment is updated. Do not open a new PR.

On the PR's **Checks** tab you see one check per topic. Click a check, then
**Summary**, for an explanation: what failed, where, how to reproduce it
locally, and whether it costs points.

| Check | Question it answers | Costs points? |
|---|---|---|
| PR Validation / Validate Submission | one task, only allowed files? | if it fails, nothing is scored |
| Build / Compile | does it compile? warnings? forbidden functions? | yes |
| Tests / Unit Tests | do the tests pass? | yes (70 points) |
| Security / AddressSanitizer | memory errors (out of bounds, use after free, ...)? | yes |
| Security / UndefinedBehaviorSanitizer | undefined behaviour (overflow, bad shifts, ...)? | yes |
| Quality / Static Analysis | what does cppcheck find? | from stage 5 |
| Quality / Formatting | does the code follow the course style? | **no**, information only |
| Quality / Memory Analysis | does Valgrind find uninitialised memory? (from stage 2) | **no**, information only |
| Grading / Automated Score | the final score, the same as `make score` | this **is** the score |

The two **Security** checks explain the same test run as *Unit Tests*; a
problem they both report on the same line costs points only once.

CI also labels your PR: `task-NN`, and `passed` (60+) or `needs-fix`. After
the defense the teacher adds `defended`.

### When the score is 60+

Book your defense. After the defense the teacher **closes** the PR. PRs are
never merged, and that is normal: your work stays in your fork.

### If `main` changed while you were working

Rarely the teacher fixes a task after you started. If GitHub says your PR has
conflicts, or the teacher asks you to update:

```bash
git fetch upstream
git merge upstream/main
git push
```

---

## 3. Scoring

`make score` and CI compute the same score:

| | Points |
|---|---|
| Does not compile | **0** in total |
| Tests | **70**, proportional to the share of tests that pass |
| Code quality | **30**, minus the deductions below (never below 0) |

| Deduction | Points |
|---|---|
| Each compiler warning | −5 |
| Each cppcheck finding (from stage 5) | −3 |
| Each distinct sanitizer report (AddressSanitizer / UBSan) | −10 |
| Each forbidden function used | −15 |
| `ANSWERS.md` left empty | −10 |

**Pass threshold: 60.** Below 60 the PR check is red; at 60 or more it is
green and you can book the defense.

Checks that switch on as you progress:

| Check | From stage |
|---|---|
| `malloc` / `calloc` / `realloc` / `free` forbidden | 3 |
| `-Wconversion` (implicit narrowing is a warning) | 4 |
| cppcheck findings cost points (before: notes only) | 5 |

Some tasks forbid more functions (e.g. `strlen` in task 06); the task's README
says which.

Good to know:

- The tests run with **AddressSanitizer** and **UBSan**, which find memory
  errors and undefined behaviour at the exact line. `make test` shows the full
  reports. Each **distinct** problem counts once, however often it happens.
- Optional extra feedback, never part of the score: `make format` shows how
  the course style (`.clang-format`) would format your files (it changes
  nothing; `clang-format -i src/file.c` applies it), and `make memcheck` runs
  the tests under Valgrind, which also finds reads of uninitialised memory.
- Every test program is stopped after **5 seconds**. A test that never
  finishes counts as failed ("infinite loop or too slow").
- A test that crashes counts as failed; the other tests still run.
- How the CI score and the defense combine into your final grade is decided
  by the teacher; CI only produces the score.

---

## 4. Rules

- **One PR = exactly one task.** A PR that touches two task folders fails the
  check.
- **Only the task's student files.** Each task's README lists the files you
  may change (usually one `.c` file and `ANSWERS.md`). Changing anything else
  (tests, headers, Makefiles, CI, scripts, other tasks) fails the check. To
  undo such a change: `git checkout upstream/main -- <file>`, then commit.
- **Branch name:** `task-NN` (e.g. `task-07`). **PR title:** `[Task 07] - Full Name`.
- **The tutor helps, it does not solve.** Using an assistant to write your
  code for you is pointless: you will not pass the defense.
- **The defense is what counts.** You must be able to explain every line and
  every answer in `ANSWERS.md`.

---

## 5. The AI tutor

The repo contains the tutor's rules, and most assistants load them
automatically:

- **GitHub Copilot Chat** reads `.github/copilot-instructions.md`.
- Assistants that support `AGENTS.md` read that file.
- **Any other assistant:** paste the whole of [`TUTOR_RULES.md`](TUTOR_RULES.md)
  into the chat at the start of a session.

How to work with it:

- Open the task's files and tell it which task you are on (it also reads your
  branch name).
- Ask about compiler errors, sanitizer reports, failing tests and score
  deductions. It explains in Hebrew and asks what *you* think first.
- It gives hints step by step: a question, then the concept, then the line,
  then pseudo-code. That is on purpose.
- When you think you are done, it asks you two check questions. Use them to
  prepare for the defense.

Each task's `TUTOR.md` contains the tutor's notes, including the **defense
questions** the teacher will ask. Read them.

---

## 6. Tasks

| Stage | Task | Topic |
|---|---|---|
| **0. Toolchain** | [00-hello-toolchain](tasks/00-hello-toolchain/) | Multi-file program, header, your own Makefile |
| | [01-first-pr](tasks/01-first-pr/) | A tiny function and the full PR flow |
| **1. C basics** | [02-fixed-width-types](tasks/02-fixed-width-types/) | `stdint.h`, saturating add/sub |
| | [03-integer-math](tasks/03-integer-math/) | Integer-only conversions with correct rounding |
| | [04-enums-and-switch](tasks/04-enums-and-switch/) | Classify commands into an enum |
| **2. Arrays & pointers** | [05-out-params](tasks/05-out-params/) | Output parameters and error codes |
| | [06-my-string](tasks/06-my-string/) | Your own `strlen`, bounded copy, compare |
| | [07-command-parser](tasks/07-command-parser/) | Parse `LED=1` lines into a struct |
| **3. Memory** | [08-memory-bugs](tasks/08-memory-bugs/) | Find and fix bugs with AddressSanitizer |
| | [09-static-pool](tasks/09-static-pool/) | Fixed-size object pool allocator |
| **4. Bits & registers** | [10-bit-macros](tasks/10-bit-macros/) | Set/clear/toggle/read, field get/set |
| | [11-register-decode](tasks/11-register-decode/) | Decode a 32-bit status register |
| | [12-endianness](tasks/12-endianness/) | Big- and little-endian byte buffers |
| | [13-crc8](tasks/13-crc8/) | CRC-8 checksum |
| **5. Embedded data structures** | [14-ring-buffer](tasks/14-ring-buffer/) | Circular FIFO buffer |
| | [15-debounce](tasks/15-debounce/) | Tick-driven button debounce |
| | [16-packet-parser](tasks/16-packet-parser/) | Byte-by-byte packet framing |
| | [17-fixed-point](tasks/17-fixed-point/) | Q16.16 arithmetic |
| **6. Logic vs. hardware** | [18-function-pointers](tasks/18-function-pointers/) | Function pointers (gentle intro) |
| | [19-hal-interface](tasks/19-hal-interface/) | GPIO HAL interface and a mock |
| | [20-traffic-light](tasks/20-traffic-light/) | Non-blocking state machine over a mock HAL |
| | [21-simulated-isr](tasks/21-simulated-isr/) | Simulated interrupt, ring buffer, `volatile` |

Do them in order: each task builds on the previous ones.

---

## 7. Troubleshooting

- **`make: command not found` / `gcc: not found`**: install the tools from
  §1.2 inside Ubuntu.
- **VS Code does not show "WSL: Ubuntu"**: close it and run `code .` from the
  Ubuntu terminal inside the repo.
- **Very slow builds**: you are probably working under `/mnt/c/...`. Clone
  into your Linux home (`~`) instead.
- **AddressSanitizer prints `DEADLYSIGNAL` endlessly, even for correct code**:
  a known clash between older GCC and newer WSL kernels. Run
  `sudo sysctl vm.mmap_rnd_bits=28` once per WSL session.
- **The PR check says "not allowed"**: see §4 on how to undo changes to
  files you may not change.

## Repository layout (for the curious)

```
common.mk        shared build logic (make, make test, make score)
scripts/         scoring and PR checks used by CI
lib/unity/       Unity test framework (MIT license)
hal/             mock hardware layer used in stage 6
tasks/NN-name/   one folder per task
```
