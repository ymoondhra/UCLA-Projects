#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <map>
using namespace std;

class TranslatorImpl
{
public:
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
private:
    vector<map<char, char>> maps;
    map<char,char> currMapTable;
    int numTimesPopped = 0;
    int numTimesPushed = 0;
    
    bool containsNonLetter(string input);
    bool isInconsistent(string ciphertext, string plaintext);
    void addToCurrMap(string ciphertext, string plaintext);
};

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext) //ciphertext is encrypted
{
    if((ciphertext.length() != plaintext.length()) || containsNonLetter(ciphertext) || containsNonLetter(plaintext) || isInconsistent(ciphertext, plaintext))
    {
        return false;
    }
    
    map<char,char> newestMap;
    for(int i = 0; i < ciphertext.length(); i++)
    {
        ciphertext[i] = tolower(ciphertext[i]);
        plaintext[i] = tolower(plaintext[i]);
        newestMap[ciphertext[i]] = plaintext[i];
    }
    maps.push_back(newestMap);
    numTimesPushed++;
    ///ADD NEW CHARACTERS TO CURRENT MAP?? OR SET CURRENT MAP EQUAL TO THIS NEW MAP?
    addToCurrMap(ciphertext, plaintext);
    return true;
}

bool TranslatorImpl::popMapping()
{
    if(numTimesPopped == numTimesPushed)
        return false;
    
    if(!maps.empty())
        maps.pop_back();
    if(maps.empty())
        currMapTable.clear();
    else
        currMapTable = maps.back();
    numTimesPopped++;
    return true;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
    string returnableString = "";
    for(int i = 0; i < ciphertext.length(); i++)
    {
        map<char,char>::const_iterator it = currMapTable.find(tolower(ciphertext[i]));
        if(isalpha(ciphertext[i])) ///OR IS IT NULLPTR??
        {
            if(it != currMapTable.end())
            {
                if(isupper(ciphertext[i]))
                    returnableString += toupper(it->second);
                else
                    returnableString += it->second;
            }
            else //if there is no decoding for that letter
            {
                returnableString += '?';
            }
        }
        else //if that char is not a letter
        {
            returnableString += ciphertext[i];
        }
    }
    return returnableString;
}

bool TranslatorImpl::containsNonLetter(string input)
{
    for(int i = 0; i < input.length(); i++)
    {
        if(isalpha(input[i]) == 0)
            return true;
    }
    return false;
}

bool TranslatorImpl::isInconsistent(string ciphertext, string plaintext)
{
    for(int i = 0; i < ciphertext.length(); i++)
    {
        for(int j = 0; j < maps.size(); j++)
        {
            if(maps[j].find(tolower(ciphertext[i])) != maps[j].end())
            {
                if((maps[j].find(tolower(ciphertext[i]))->second != tolower(plaintext[i])))
                    return true;
            }
        }
        
        for(int j = 0; j < maps.size(); j++)
        {
            for(map<char,char>::iterator it = maps[j].begin(); it != maps[j].end(); ++it)
            {
                if(it->second == tolower(plaintext[i]))
                {
                    if(it->first != tolower(ciphertext[i]))
                        return true;
                }
            }
        }
    }
    return false;
}

void TranslatorImpl::addToCurrMap(string ciphertext, string plaintext)
{
    for(int i = 0; i < ciphertext.length(); i++)
    {
        currMapTable[tolower(ciphertext[i])] = tolower(plaintext[i]);
    }
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}
