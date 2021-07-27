#include "Lexer.h"

#include <sstream>
#include <iostream>
#include <algorithm>

#include "Token.h"

using namespace std;

void Lexer::SetText(const std::string& text)
{
    if(text.empty())
    {
        error = "Empty source code";
        return;
    }
    std::string data = text;
    std::transform(data.begin(), data.end(), data.begin(),
        [](unsigned char c){ return std::tolower(c); });
    while(!m_tokens.empty())
          m_tokens.pop();
    unsigned pivot = 0;
    Token token;
    while(token != Token::Type::End)
    {
        token = ParseNextToken(data, pivot);
        m_tokens.push(token);
    }
    m_tokens.push(token);
}

Token Lexer::NextToken()
{
    if(!m_tokens.empty())
    {
        auto first = m_tokens.front();
        m_tokens.pop();
        return first;
    }
    return Token(Token::Type::End);
}

string Lexer::GetError()
{
    return error;
}

bool Lexer::IsError()
{
    return !error.empty();
}

Token Lexer::ParseNextToken(const std::string& data, unsigned& pivot)
{
    while (pivot < data.size() && isspace(data[pivot]))
        pivot++;
    if(data[pivot] == '/' && data[pivot+1] == '/')
    {
        while(pivot < data.size() && data[pivot] != '\n')
            pivot++;
        pivot++;
        return ParseNextToken(data, pivot);
    }

    if(pivot >= data.size() || data[pivot] == '\0')
        return Token(Token::Type::End, "\\0");
    if(data[pivot] == ';')
    {
        pivot++;
        return Token(Token::Type::Endline, ';');
    }
    unsigned begin = pivot;
    if ((data[pivot+1] == '.')
            || isdigit (data[pivot])
            // allow decimal numbers without a leading 0. e.g. ".5"
            || (data[pivot] == '.' && isdigit (data[pivot+1])))
    {
        // skip sign for now
        if ((data[pivot] == '+' || data[pivot] == '-'))
            pivot++;
        while (isdigit (data[pivot]) || data[pivot] == '.')
            pivot++;
        // allow for 1.53158e+15
        if (data[pivot] == 'e' || data[pivot] == 'E')
        {
            pivot++; // skip 'e'
            if ((data[pivot]  == '+' || data[pivot]  == '-'))
                pivot++; // skip sign after e
            while (isdigit (data[pivot]))  // now digits after e
                pivot++;
        }
        string number = data.substr(begin, pivot - begin);
        double val;
        istringstream is (number);
        is >> val;

        if (is.fail () && !is.eof ())
        {
            error = "Unexpected eof";
            return Token(Token::Type::End);
        }
        return Token(Token::Type::Number, val);
    }

    char symbol = data[pivot++];
    switch (symbol)
    {
    case '=':
        return Token(Token::Type::Assign, symbol);
    case '(':
        return Token(Token::Type::ParenOpen, symbol);
    case ')':
        return Token(Token::Type::ParenClose, symbol);
    case '+':
        return Token(Token::Type::Plus, symbol);
    case '-':
        return Token(Token::Type::Minus, symbol);
    case '*':
        return Token(Token::Type::Multiply, symbol);
    case '/':
    case '\\':
        return Token(Token::Type::Divide, symbol);
    case '&':
        return Token(Token::Type::Union, symbol);
    case '|':
        return Token(Token::Type::Cross, symbol);
    case ',':
        return Token(Token::Type::Comma, symbol);
    case '^':
        return Token(Token::Type::Pow, symbol);
    }
    pivot--;

    if (!isalpha (data[pivot]))
    {
        error = "Unexpected symbol "+string(1, data[pivot]);
        return Token(Token::Type::End);
    }

    // we have a word (starting with A-Z) - pull it out
    while (isalnum (data[pivot]) || data[pivot] == '_')
        pivot++;
    auto word = data.substr(begin, pivot-begin);
    return Token(Token::Type::Id, word);
}
