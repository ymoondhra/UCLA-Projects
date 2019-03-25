#include "provided.h"
#include <string>
#include <vector>
#include <cctype>
using namespace std;

class TokenizerImpl
{
public:
    TokenizerImpl(string separators);
    vector<string> tokenize(const std::string& s) const;
private:
    string wordSeparators;
};

TokenizerImpl::TokenizerImpl(string separators)
{
    wordSeparators = separators;
}

vector<string> TokenizerImpl::tokenize(const std::string& s) const
{
    vector<string> tokenizedWords;
    string temp = "";
    bool isSeparator;
    for(int i = 0; i < s.length(); i++)
    {
        isSeparator = false;
        for(int j = 0; j < wordSeparators.length(); j++)
        {
            if(wordSeparators[j] == s[i])
                isSeparator = true;
        }
        if(isSeparator)
        {
            if(temp != "")
            {
                tokenizedWords.push_back(temp);
                temp = "";
            }
        }
        else //is not a separator
        {
            temp += s[i];
        }
    }
    if(temp != "")
        tokenizedWords.push_back(temp);
    
    return tokenizedWords;
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
    m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
    delete m_impl;
}

vector<string> Tokenizer::tokenize(const std::string& s) const
{
    return m_impl->tokenize(s);
}
