#include "Parser.h"

#include "LangFunctions.h"
#include "Token.h"

#include <sstream>
#include <fstream>


using namespace std;


Parser::Parser()
{

}


void Parser::SetFile(const std::string &sourceFile)
{
    fstream file(sourceFile);
    if(!file)
    {
        cout<<"Parser: couldn't open file"<<endl;
        return;
    }
    stringstream converter;
    converter<<file.rdbuf();

    file.close();
    SetText(converter.str());
}

void Parser::SetText(const std::string &source)
{
    lexer.SetText(source);
    token.type = Token::Type::Begin;
}

Program *Parser::GetProgram(SymbolTable* baseTable)
{
    Program* program = new Program;
    if(baseTable)
        program->GetSymbolTable().Concat(*baseTable);
    SymbolTable& table = program->GetSymbolTable();
    ToNextToken();
    if(!lexer.IsError() && !IsError())
    {
        while(token != Token::Type::End)
        {
            if(token == Token::Type::Id)
            {
                if((token.name == "variable" ||
                        token.name == "var"))
                {
                    auto expr = HandleVariable(table);
                    if(expr)
                        table.Add(expr);
                }
                else if(token.name == "argument" ||
                        token.name == "arg")
                {
                    HandleArgument(table);
                }
                else if(token.name == "constant" ||
                        token.name == "const")
                {
                    HandleConstant(table);
                }
                else if(token.name == "return")
                {
                    auto expr = HandleReturn(table);
                    program->SetResult(expr);
                }
                else
                {
                    error = "Unknown keyword " + token.name;
                }
            }
            else
            {
                error = "Unknown token " + token.name;
            }
        }
    }
    else
    {
        if(lexer.IsError())
            cout<<"Lexer error: "<<lexer.GetError()<<endl;
        else
            cout<<"Parser error: "<<error<<endl;
        delete program;
        program = nullptr;
    }

    return program;
}

bool Parser::IsError()
{
    return !error.empty();
}

string Parser::GetError()
{
    return error;
}

void Parser::HandleArgument(SymbolTable &table)
{
    Expression* expr = nullptr;
    if(!IsError())
    {
        while(token != Token::Type::Endline)
        {
            ToNextToken();
            CheckToken(Token::Type::Id);
            string name = token.name;
            ToNextToken();
            pair<double, double> limit{-5, 5};
            if(token == Token::Type::ParenOpen)
            {
                ToNextToken();
                bool negative = false;
                if(token == Token::Type::Minus)
                {
                    negative = true;
                    ToNextToken();
                }
                CheckToken(Token::Type::Number);
                limit.first = token.value;
                ToNextToken();
                if(token == Token::Type::Comma)
                {
                    limit.first = negative ? -limit.first : limit.first;
                    ToNextToken();
                    negative = false;
                    if(token == Token::Type::Minus)
                    {
                        negative = true;
                        ToNextToken();
                    }
                    CheckToken(Token::Type::Number);
                    limit.second = negative ? -token.value : token.value;
                    ToNextToken();
                }
                else
                {
                    limit.second = limit.first;
                    limit.first = -limit.second;
                }
                CheckToken(Token::Type::ParenClose);
                ToNextToken();
            }
            expr =  NodeCreator::Instance().Create<ArgumentExpr>(name, limit);
            table.Add(expr);
        }
        CheckToken(Token::Type::Endline);
        ToNextToken();
    }
    else
        cout<<"Parser error: "<<error;
}


void Parser::HandleConstant(SymbolTable &table)
{
    Expression* expr = nullptr;
    if(!IsError())
    {
        while(token != Token::Type::Endline)
        {
            ToNextToken();
            CheckToken(Token::Type::Id);
            string name = token.name;
            ToNextToken();
            CheckToken(Token::Type::Assign);
            ToNextToken();
            auto expr = Expr(table);
            expr = NodeCreator::Instance().Create<ConstExpr>(name, expr);
            table.Add(expr);
        }
        ToNextToken();
    }
    else
        cout<<"Parser error: "<<error;
}

Expression* Parser::HandleVariable(SymbolTable &table)
{
    Expression* expr = nullptr;
    if(!IsError())
    {
        ToNextToken();
        CheckToken(Token::Type::Id);
        string name = token.name;
        ToNextToken();
        CheckToken(Token::Type::Assign);
        ToNextToken();
        expr = Expr(table);
        expr = NodeCreator::Instance().Create<VariableExpr>(name, expr);
        ToNextToken();
    }
    else
        cout<<"Parser error: "<<error;
    return expr;
}

Expression *Parser::HandleReturn(SymbolTable &table)
{
    Expression* expr = nullptr;
    if(!IsError())
    {
        ToNextToken();
        expr = Expr(table);
        ToNextToken();
    }
    else
        cout<<"Parser error: "<<error;
    return expr;
}

void Parser::CheckToken(Token::Type expect)
{
    if(token != expect)
    {
        cout<<"Parser error: \n"<<"Unexpected token "+token.ToString()+
              "\n"+"Expected " + Token(expect).ToString()<<endl;
    }
}

Expression* Parser::Term(SymbolTable &table)
{
    auto node = Factor(table);
    while(token == Token::Type::Pow      ||
          token == Token::Type::Multiply ||
          token == Token::Type::Divide)
    {
        auto prev = token;
        ToNextToken();
        node = NodeCreator::Instance().Create<BinaryExpr>(prev.name, node, Factor(table));
    }
    return node;
}

Expression* Parser::Factor(SymbolTable &table)
{
    if(token == Token::Type::Number)
    {
        auto expr = NodeCreator::Instance().Create<NumberExpr>(token.value);
        ToNextToken();
        return expr;
    }
    else if(token == Token::Type::ParenOpen)
    {
        ToNextToken();
        auto expr = Expr(table);
        ToNextToken();
        return expr;
    }
    else if(token.type == Token::Type::Minus)
    {
        ToNextToken();
        return NodeCreator::Instance().Create<UnaryExpr>("-", Factor(table));
    }
    else if(token.type == Token::Type::Id)
    {
        auto prev = token;
        ToNextToken();
        Expression* expr = table.GetArgument(prev.name);
        if(expr)
            return expr;
        expr = table.GetConst(prev.name);
        if(expr)
            return expr;
        expr = table.GetVariable(prev.name);
        if(expr)
            return expr;
        auto func = LangFunctions::FindFunction(prev.name);
        if(func)
            return NodeCreator::Instance().Create<FunctionExpr>(func, Term(table));
    }
    return nullptr;
}

Expression* Parser::Expr(SymbolTable &table)
{
    auto node = Term(table);
    while(token == Token::Type::Minus||
          token == Token::Type::Plus ||
          token == Token::Type::Cross ||
          token == Token::Type::Union)
    {
        auto prev = token;
        ToNextToken();
        node = NodeCreator::Instance().Create<BinaryExpr>(prev.name, node, Term(table));
    }
    return node;
}

void Parser::ToNextToken()
{
    token = lexer.NextToken();
    if(lexer.IsError())
        error = "Next token error";
}
