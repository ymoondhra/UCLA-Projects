#include "provided.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

class DecrypterImpl
{
public:
    DecrypterImpl();
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
    WordList wL;
    Tokenizer tok; 
    Translator trannie;
    string punctuation;
    
    string nextWord(vector<string> v) const;
    int numCharsNotKnown(string s) const;
    bool hasQuestionMark(string s) const;
    bool alreadyChosen(string s) const;
    bool isFullyTranslated(vector<string> input) const;
    bool allTranslatedWordsAreInDict(vector<string> input) const;
    bool isLower(string a, string b) const;
};

DecrypterImpl::DecrypterImpl()
:tok(" ,;:.!()[]{}-\"#$%^&0123456789")
{
    punctuation = " ,;:.!()[]{}-\"#$%^&0123456789";
}


bool DecrypterImpl::load(string filename)
{
    if(wL.loadWordList(filename))
        return true;
    return false;  // This compiles, but may not be correct
}

vector<string> DecrypterImpl::crack(const string& ciphertext)
{
    string original = ciphertext;
    
    vector<string> finalCandidates;
    vector<string> tokenizedWords = tok.tokenize(ciphertext); //Step 2 part a: break up the word
    string currentTokenizedWord = nextWord(tokenizedWords); //Step 2 part c: pick a word that has the most cipher letters for which we don't have any translation
    string currTranslation = trannie.getTranslation(currentTokenizedWord); //Step 3: translate this word
    
    vector<string> candidatesForThisWord = wL.findCandidates(currentTokenizedWord, currTranslation); //Step 4: find candidates function of wordList
    
    if(candidatesForThisWord.empty())
    {
        trannie.popMapping();
        return candidatesForThisWord;
    }
    
    //Step 6: for each candidate for this word...
    for(int i = 0; i < candidatesForThisWord.size(); i++)
    {
        if(trannie.pushMapping(currentTokenizedWord, candidatesForThisWord[i]))
            {
                string temp = trannie.getTranslation(ciphertext);
                vector<string> newTokenized = tok.tokenize(trannie.getTranslation(ciphertext));
                bool allTranslatedWordsAreInWL = true;
                bool fullyTranslated = isFullyTranslated(newTokenized);
                for(int q = 0; q < newTokenized.size(); q++)
                {
                    if(!hasQuestionMark(newTokenized[q]))
                    {
                        if(!wL.contains(newTokenized[q]))
                        {
                            allTranslatedWordsAreInWL = false;
                        }
                    }
                }
                
                if(!allTranslatedWordsAreInWL)
                {
                    trannie.popMapping();
                }
                else if(allTranslatedWordsAreInWL && !fullyTranslated)
                {
                    vector<string> newFinals = crack(ciphertext);
                    
                    for(int y = 0; y < newFinals.size(); y++)
                    {
                        vector<string> sentence = tok.tokenize(newFinals[y]);
                        if(isFullyTranslated(sentence) && allTranslatedWordsAreInDict(sentence))
                        {
                            finalCandidates.push_back(newFinals[y]);
                        }
                    }
                    trannie.popMapping();
                }
                else if(allTranslatedWordsAreInWL && fullyTranslated)
                {
                    finalCandidates.push_back(temp);
                }
            }
    }
    
    trannie.popMapping();
    
    if(finalCandidates.size() > 1)
        sort(finalCandidates.begin(), finalCandidates.end());
    return finalCandidates; //Step 8: return to the previous recursive call
}

string DecrypterImpl::nextWord(vector<string> v) const
{
    string s = "";
    for(int q = 0; q < v.size(); q++)
    {
        if( (!alreadyChosen(v[q])) && (numCharsNotKnown(v[q]) > numCharsNotKnown(s)) )
            s = v[q];
    }
    return s;
}

int DecrypterImpl::numCharsNotKnown(string s) const
{
    int count = 0;
    string translation = trannie.getTranslation(s);
    for(int j = 0; j < translation.size(); j++)
    {
        if(translation[j] == '?')
            count++;
    }
    return count;
}

bool DecrypterImpl::hasQuestionMark(string s) const
{
    for(int i = 0; i < s.length(); i++)
    {
        if(s[i] == '?')
            return true;
    }
    return false;
}

bool DecrypterImpl::alreadyChosen(string s) const
{
    if(hasQuestionMark(s))
        return true;
    return false;
}

bool DecrypterImpl::isFullyTranslated(vector<string> sentence) const
{
    for(int i = 0; i < sentence.size(); i++)
    {
        if(hasQuestionMark(sentence[i]))
            return false;
    }
    return true;
}

bool DecrypterImpl::allTranslatedWordsAreInDict(vector<string> sentence) const
{
    for(int i = 0; i < sentence.size(); i++)
    {
            if(!hasQuestionMark(sentence[i]) && !wL.contains(sentence[i]))
                return false; //is a full word but is not in word liss
    }
    return true;
}

bool DecrypterImpl::isLower(string a, string b) const
{
    for(int i = 0; i < a.size() && i < b.size(); i++)
    {
        if(tolower(a[i]) != tolower(b[i]) && isalpha(a[i]) && isalpha(b[i]))
        {
            return tolower(a[i]) < tolower(b[i]);
        }
    }
    return false;
}


//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
    m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
    delete m_impl;
}

bool Decrypter::load(string filename)
{
    return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
   return m_impl->crack(ciphertext);
}
