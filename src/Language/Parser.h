#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"
#include "Program.h"
#include "Token.h"

// Класс, который создает абстрактное синтаксическое дерево из токенов Лексера
class Parser
{
public:
    Parser();

    void SetFile(const std::string& sourceFile);
    void SetText(const std::string& source);

    Program* GetProgram(SymbolTable *baseTable = nullptr);

    bool IsError();
    std::string GetError();

private:
    Lexer lexer;
    Token token;

    std::string error = "";

    // Функции обработки главных элементов языка
    void HandleArgument(SymbolTable& table);
    void HandleConstant(SymbolTable& table);
    Expression* HandleVariable(SymbolTable& table);
    Expression* HandleReturn(SymbolTable& table);

    void CheckToken(Token::Type expect);

    // рекурсивные функции построения дерева
    Expression *Term(SymbolTable& table);
    Expression *Factor(SymbolTable& table);
    Expression *Expr(SymbolTable& table);

    void ToNextToken();
};

#endif // PARSER_H
