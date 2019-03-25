# UCLA Projects
This is a collection of some of my best coding projects assigned by UCLA courses.

## Software Construction Laboratory – CS 35L ##


## Computer Organization – CS33 ##

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


## Data Structures & Algorithms – CS32 ##

