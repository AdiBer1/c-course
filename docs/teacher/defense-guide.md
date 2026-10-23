# Oral defense guide (teacher)

About **5 minutes** per task, after the PR has reached 60+.

## Before (1 minute, alone)

- Open the PR: score comment, *Files changed*, *Commits*.
- Open the task's `TUTOR.md` → **Defense questions**, and the student's
  `ANSWERS.md` in the PR.
- Note one line of her code you want explained (something non-trivial: a
  bound, a cast, a condition).

## During (about 5 minutes)

1. **Walk-through (≈ 2 min).** She shares her screen (or the PR diff) and
   explains her code from top to bottom, in her own words. Interrupt with
   "why?" at the line you picked.
2. **Defense questions (≈ 2 min).** Ask 2–3 questions from the task's
   `TUTOR.md`. Students know these questions in advance; listen for
   understanding, not memorised phrases. A good follow-up is "and what if...?"
   (a different input, a 16-bit `int`, an interrupt at that moment).
3. **ANSWERS.md (≈ 1 min).** Pick one of her answers and ask her to explain
   it differently, or to apply it to a new example.
4. **Commit history (a glance).** Steady progress in several commits is
   normal. One big commit with perfect code is not proof of anything, but it
   is a reason to probe more deeply.

## Signals

| Good | Worrying |
|---|---|
| Explains *why*, not just *what* | Reads the code aloud without explaining |
| Can predict what happens for a new input | Cannot say what a line does without running it |
| Admits uncertainty and reasons it out | Answers sound memorised, collapse on a follow-up |
| Mentions the embedded reason (RAM, ISR, no FPU...) | Cannot connect the task to hardware at all |

## After

- Write one line in the PR (date, outcome), then **Close** the PR (never merge).
- Record the result in your own grade sheet. How the CI score and the defense
  combine into the final grade is your decision; the repository only produces
  the CI score.
