#ifndef TOKEN_H
#define TOKEN_H

#include <string>


// Атоманрая единица языка
struct Token
{
    enum class Type
    {
        Begin,
        Id,
        Number,
        Assign,
        Endline,
        End,
        ParenOpen,
        ParenClose,
        Comma,
        Pow,
        Multiply,
        Divide,
        Plus,
        Minus,
        Cross,
        Union
    };

    Type type;
    double value = 0;
    std::string name = "";

    Token(Type type = Type::Begin):
        type(type){}
    Token(Type type, double value):
        type(type),
        value(value){}
    Token(Type type, std::string name):
        type(type),
        name(name){}
    Token(Type type, char c):
        type(type),
        name(std::string(1, c)){}

    std::string ToString()
    {
        switch (type) {
        case Type::Begin:
            return "Type::Begin";
        case Type::Id:
            return "Type::Id : "+name;
        case Type::Number:
            return "Type::Number : "+std::to_string(value);
        case Type::Endline:
            return "Type::Endline";
        case Type::End:
            return "Type::End";
        case Type::ParenOpen:
            return "Type::ParenOpen";
        case Type::ParenClose:
            return "Type::ParenClose";
        case Type::Comma:
            return "Type::Comma";
        case Type::Pow:
            return "Type::Pow";
        case Type::Multiply:
            return "Type::Multiply";
        case Type::Divide:
            return "Type::Divide";
        case Type::Plus:
            return "Type::Plus";
        case Type::Minus:
            return "Type::Minus";
        case Type::Cross:
            return "Type::Cross";
        case Type::Union:
            return "Type::Union";
        case Type::Assign:
            return "Type::Assign";
        }
    }

    bool operator !=(Type type)
    {
        return this->type != type;
    }
    bool operator ==(Type type)
    {
        return this->type == type;
    }
};

#endif // TOKEN_H
