#ifndef LANGFUNCTIONS_H
#define LANGFUNCTIONS_H

#include <map>

typedef double(*FunctionRef)(double);
typedef double(*BinaryOp)(double, double);
typedef double(*UnaryOp)(double);

// Класс, который содержит информацию о функциях языка Ранока
class LangFunctions
{
public:
    LangFunctions() = delete;
    static std::map<std::string, FunctionRef> functions;
    static std::map<std::string, BinaryOp> binOperations;
    static std::map<std::string, UnaryOp> unaryOperations;


    static FunctionRef FindFunction(std::string);
    static std::string FindFunction(FunctionRef);

    static BinaryOp FindBinaryOp(std::string);
    static std::string FindBinaryOp(BinaryOp);

    static UnaryOp FindUnaryOp(std::string);
    static std::string FindUnaryOp(UnaryOp);

    static std::string FindFunctionCodeName(std::string name);
};

#endif // LANGFUNCTIONS_H
