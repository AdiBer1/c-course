# Task 00 — Understanding questions
Answer in your own words, a few sentences each. Write under each Answer: line.
There is no code to write here; these are about why.
## Q1. gcc -c mathx.c produces mathx.o, but that file cannot run. What is missing, and which step adds it?
Answer:
The .o file contains the compiled code but it still needs to be linked with the other .o files
## Q2. What would go wrong if mathx.h had no include guard and some .c file ended up including it twice (directly or through another header)? Would it still go wrong if the header only contained function declarations?
Answer:
If it contained declerations that cant be repeated it can cause compilation errors. In our case it conatins only function declerations that can be repeated 
but the include gaurd is still usefull since it protects against future changes in the header.
## Q3. You change only mathx.c. Which files does your Makefile rebuild, and why is that better than recompiling everything — especially on a big firmware project?
Answer:
We will need to rebuild only mathx.o and then link main.o and mathx.o again to create hello.
It saves time because we recompile only the files that were changed or imapacted by the change.
