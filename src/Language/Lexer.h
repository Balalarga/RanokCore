#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <queue>

class Token;

// Класс, разделяющий исходный код на поток Токенов
class Lexer
{
public:
    Lexer() = default;
    ~Lexer() = default;

    void SetText(const std::string& text);

    Token NextToken();

    std::string GetError();
    bool IsError();

protected:
    Token ParseNextToken(const std::string &data, unsigned &pivot);

private:
    std::queue<Token> m_tokens;
    std::string error = "";
};

#endif // LEXER_H
