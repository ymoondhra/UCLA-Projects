#include "provided.h"
#include "MyHash.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class WordListImpl
{
public:
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;
private:
    string createWordPattern(string input) const;
    bool hasSameWordPattern(string a, string pattern) const;
    bool isCandidate(string cipherWord, string currTranslation, string key) const;
    
    MyHash<string, vector<string>> hash;
};

bool WordListImpl::loadWordList(string filename)
{
    //deleting old data
    hash.reset();
    
    //converting .txt file into a binary tree data structure
    ifstream infile(filename);
    if( ! infile )
        return false;
    
    string s;
    // getline returns infile; the while tests its success/failure state
    while (getline(infile, s)) //while there are words remaining, add each one to the hash
    {
        bool isValid = true;
        for(int i = 0; i < s.size(); i++)
        {
            if((isalpha(s[i]) == 0) && (s[i] != '\''))
            {
                isValid = false;
                break;
            }
        }
        if(isValid)
        {
            string wordPatternOfS = createWordPattern(s);
            vector<string>* v = hash.find(wordPatternOfS);
            vector<string> newValue;
            if(v != nullptr)
               newValue = *v;
            bool add = true;
            for(int i = 0; i < newValue.size(); i++)
            {
                if(newValue[i] == s)
                   add = false;
            }
            if(add)
               {
                   for(int i = 0; i < s[i]; i++)
                   {
                       s[i] = tolower(s[i]);
                   }

                   newValue.push_back(s);
                   hash.associate(wordPatternOfS, newValue);
                   
               }
        }
    }
    return true;
}

bool WordListImpl::contains(string word) const
{
    for(int i = 0; i < word.length(); i++)
    {
        word[i] = tolower(word[i]);
    }
    const vector<string>* vec = hash.find(createWordPattern(word));
    if(vec == nullptr) //if it is a nullptr, it was not found, so return false
        return false;
    else
    {
        for(vector<string>::const_iterator it = (*vec).begin(); it != (*vec).end(); ++it)
        {
            if((*it) == word)
                return true;
        }
        return false;
    }
}
vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
    vector<string> candidates;
    const vector<string>* vec = hash.find(createWordPattern(cipherWord));
    if(vec != nullptr)
        candidates = *vec;
    else
        return candidates;
    
    ///DO I NEED TO CHECK THAT THE CANDIDATES HAVE THE SAME NUMBER OF LETTERS AS THE CIPHERWORD?
    
    //Checking if each candidate follows the current translation
    vector<string>::iterator it = candidates.begin();
    while(it != candidates.end())
    {
        if(hasSameWordPattern((*it), currTranslation) && isCandidate(cipherWord, currTranslation, *it))
            ++it;
        else
            it = candidates.erase(it);
    }
    return candidates;
}

bool WordListImpl::hasSameWordPattern(string a, string pattern) const
{
    for(int i = 0; (i < a.length() && i < pattern.length()); i++)
    {
        if(pattern[i] != '?')
        {
            if(tolower(a[i]) != tolower(pattern[i]))
                return false;
        }
    }
    return true;
}

string WordListImpl::createWordPattern(string input) const
{
    string pattern(input.size(), '?');
    int count = 'a';
    for(int i = 0; i < input.length(); i++)
    {
        if(pattern[i] == '?')
        {
            pattern[i] = (char) count;
            for(int j = i + 1; j < input.length(); j++)
            {
                if(tolower(input[j]) == tolower(input[i]))
                    pattern[j] = pattern[i];
            }
            count++;
        }
    }
    return pattern;
}

//checks if the inputted string input has the same numer of letters as the cipherWord and follows the currTranslation.
//all the inputted candidate words we already know follow the same letter pattern as the cipherWord
bool WordListImpl::isCandidate(string cipherWord, string currTranslation, string input) const
{
    bool sameNumLetters = cipherWord.length() == currTranslation.length();
    bool inLineWithCurrTrans = true;
    
    //checks if is in line with the current translation
    for(int j = 0; j < currTranslation.length(); j++)
    {
        if(currTranslation[j] != '?')
        {
            string tm = input;
            if(tolower(tm[j]) != tolower(currTranslation[j]))
                inLineWithCurrTrans = false;
        }
    }
    bool isCandidate = sameNumLetters && inLineWithCurrTrans;
    return isCandidate;
}

//***** hash functions for string, int, and char *****

unsigned int hash(const std::string& s)
{
    return std::hash<std::string>()(s);
}

unsigned int hash(const int& i)
{
    return std::hash<int>()(i);
}

unsigned int hash(const char& c)
{
    return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
    m_impl = new WordListImpl;
}

WordList::~WordList()
{
    delete m_impl;
}

bool WordList::loadWordList(string filename)
{
    return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
    return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
   return m_impl->findCandidates(cipherWord, currTranslation);
}
