# UCLA Projects
This is a collection of some of my best coding projects assigned by UCLA courses.

## Software Construction Laboratory – CS 35L ##
### Assignment 10: Research and development in computing ###
* Goal: Understand how a recent computer science development functions and analyze the implications of its creation
* Method: Wrote a research paper and did a ten-minute presentation in front of my class about a sonnet-generating algorithm created at Duke University

### Assignment 9: Change management ###
* Goal: Master version control by editing the changelog, using the functionality of branches, and implementing patches
* Method: Downloaded a version of a library called "diffutils" and applied changes in a multitude of ways (e.g. through emacs)
* Language(s): Git, Bash

### Assignment 8: SSH setup and use in applications ###
* Goal: Experience asymmetric cryptography by "hacking" into someone's BeagleBone and learn how to create public keys
* Method: Logged into my partner's BeagleBone, managed authentication using ssh-agent, and ran commands on his BeagleBone
* Language(s): Bash

### Assignment 7: Dynamic linking ###
* Goal: Learn how to create a makefile and attach dynamic libraries to C programs
* Method: Dismantled a C program that generates random numbers, attached dynamic libraries to it, and assembled everything by creating a makefile
* Language(s): C, Bash

### Assignment 6: Multithreaded performance ###
* Goal: Learn how to maximize the efficiency of C programs by utilizing threads
* Method: Disassembled a ray tracing C program and increased the speed by 400% (1 thread vs 8 threads)
* Language(s): C, Bash

### Assignment 5: System call programming and debugging ###
* Goal: Master the usage of buffered and unbuffered I/O by using system calls and the get/putchar functions
* Method: Created a program identical to that of Assignment 4 except by using system calls and adding functionality. Analyzed the system calls using strace
* Language(s): C, Bash

### Assignment 4: C programming and debugging ###
* Goal: Learn how to take advantage of dynamic memory allocation in C
* Method: Create an algorithm that takes an encrypted set of data and outputs a sorted version of the data still in its encrypted form. The program had to decrypt the data, store it dynamically, sort it, and then encrypt it again. 
* Language(s): C

### Assignment 3: Modifying and rewriting software ###
* Goal: Learn how to patch files, code in Python, and use Python's ArgumentParser
* Method: Created and implemented patch files and separately built a Python program which represents the Python version of the "shuf" Bash command. It requires a text file and outputs the lines in a randomly generated order. The user is able to use numerous options to specify requirements for the output. 
* Language(s): Python, Bash

### Assignment 2: Shell scripting ###
* Goal: Master the Bash commands for searching and sorting
* Method: Created a Hawaiian spell-checker algorithm that converts a website page (as an HTML file) into a list of words and then compares the list against a Hawaiian dictionary
* Language(s): HTML, Bash

### Assignment 1: Getting to know your system ###
* Goal: Learn emacs, Bash commands, and the organization of files in the computer system
* Method: Completed numerous small tasks by navigating around my computer using the command line and by editing files using emacs
* Language(s): Bash

## Computer Organization – CS33 ##
### Bomb Lab ###
Each student was given a unique memory dump and had to figure out the input keys to defuse six bombs. To find the correct input, I set breakpoints in the program and stepped through line-by-line, checking values in registers and finding patterns in the way the code handled my input. The following is a small snippet of code I traced through.

0000000000400f3b <phase_6>: <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f3b:    41 54          &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;             push   %r12 <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f3d:    55                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;           push   %rbp <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f3e:    53                   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;        push   %rbx <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f3f:    48 83 ec 50       &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;      sub    $0x50,%rsp <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f43:    48 8d 6c 24 30  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;      lea    0x30(%rsp),%rbp <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f48:    48 89 ee           &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;       mov    %rbp,%rsi <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f4b:    e8 9c 05 00 00    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;    callq  4014ec <read_six_numbers> <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f50:    41 bc 00 00 00 00 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;   mov    $0x0,%r12d <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f56:    8b 45 00             &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;    mov    0x0(%rbp),%eax <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f59:    83 e8 01               &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;   sub    $0x1,%eax <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f5c:    83 f8 05                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;   cmp    $0x5,%eax <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f5f:    76 05                     &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;   jbe    400f66 <phase_6+0x2b> <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f61:    e8 50 05 00 00      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  callq  4014b6 <explode_bomb> <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f66:    41 83 c4 01          &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;   add    $0x1,%r12d <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f6a:    41 83 fc 06           &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;   cmp    $0x6,%r12d <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f6e:    74 22                   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;    je     400f92 <phase_6+0x57> <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f70:    44 89 e3           &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;      mov    %r12d,%ebx <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f73:    48 63 c3              &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;    movslq %ebx,%rax <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f76:    8b 55 00              &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;    mov    0x0(%rbp),%edx <br />

### Open MP Optimization Lab ###
Each student was given the same C program, created by a UCLA graduate student, that superimposes multiple X-Ray photos to create a single photo. The goal was to make the code twenty times faster than the original program. I used the OpenMP API and optimization techniques such as tiling, strength reduction, and code motion to make the code thirty one times faster. 

### Attack Lab ###
The goal of this six-phase lab was to hack a program by taking advantage of buffer overflow, injecting code, and using return-oriented-programming. I used the debugging tools GDB and OBJDUMP and used gadgets (byte-values) from a gadget farm. 

## Data Structures & Algorithms – CS32 ##
### Decrypter ###
The assignment was to build a C++ program that can decrypt messages that are encrypted with the Simple Substitution Cipher 
(i.e. grey grapefruit --> qefx qebdfvjuio). The algorithm must print all possible decryptions that the encrypted message could be
(i.e. aba could be dad, mom, pop, etc...). <br />

#### These are the classes I created: ####
* MyHash.h: A template, auto-resizing hash table class built from scratch
* Wordlist.cpp: A class called WordList maps each word pattern to the appropriate words in the dictionary using a MyHash object (i.e. abbcd maps to apple, goose, etc...) <br />
* Tokenizer.cpp: A class called Tokenizer  takes an input string and returns a vector of strings of the words of that input
(i.e. input: "abde edf gabga def" . . . returns: ["abde", "edf", "gabga", "def"]) <br />
* Translator.cpp: A class called Translator stores a vector of the possible mappings in the form of maps (vector<map<char,char>> maps). 
Has functions like "pushMapping(string, string)", which adds a map<char, char> to the vector of maps <br />
* Decrypter.cpp: A class called Decrypter that has an instance of WordList, Tokenizer, and Translator. Its main function, crack(string ciphertext) takes in an encrypted string as an input. Using the tokenizer, it separates the input string into words. The private function nextWord picks the most valuable next word in the string to find decryption mappings for based on the number of characters unknown. Using the Translator object, a string is produced based on what letters are known (i.e. a) and what letters are not (?). We then find the candidate words that the encrypted word could be. If there are no candidates for the word, we throw away that the mapping because we know it is wrong. Otherwise, we run a for loop that iterates through each word of the tokenized list pushes a possible mapping for each of the candidate words. The mapping is thrown away if the fully translated words are not in the wordlist. If the words are in the wordlist and the sentence is not fully translated (it has '?'s), then the crack function is recursively called. The fully translated mappings with words that are all in the word list are returned.

### NachenBlaster ###
We were given the setup and environment for an empty game. The assignment was to build a C++ program very similar to the video game Galaga (https://www.youtube.com/watch?v=dvjapcHsqXY), where the user shoots cabbages at alien ships. Each level becomes increasingly difficult, as the aliens move faster, gain health, and increase in speed. The user's spaceship, called the NachenBlaster, can pick up powerups such as torpedos or extra lives. <br />
To create this application, I used inheritance (Actor.h), polymorphism (Actor.cpp), and encapsulation (StudentWorld.cpp).

