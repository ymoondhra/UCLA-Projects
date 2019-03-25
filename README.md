# UCLA Projects
This is a collection of some of my best coding projects assigned by UCLA courses.

## Software Construction Laboratory – CS 35L ##


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
    &nbsp;&nbsp;&nbsp;&nbsp;400f79:    3b 54 84 30        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;    cmp    0x30(%rsp,%rax,4),%edx <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f7d:    75 05                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;   jne    400f84 <phase_6+0x49>  <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f7f:    e8 32 05 00 00      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;   callq  4014b6 <explode_bomb> <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f84:    83 c3 01                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  add    $0x1,%ebx <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f87:    83 fb 05               &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;   cmp    $0x5,%ebx <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f8a:    7e e7                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;   jle    400f73 <phase_6+0x38> <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f8c:    48 83 c5 04         &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;   add    $0x4,%rbp <br />
    &nbsp;&nbsp;&nbsp;&nbsp;400f90:    eb c4                   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;   jmp    400f56 <phase_6+0x1b> <br />


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

