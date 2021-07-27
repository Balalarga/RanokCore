#include "LangFunctions.h"
#include <cmath>

std::map<std::string, FunctionRef> LangFunctions::functions =
{
    {"abs",    std::abs},
    {"sqrt",   std::sqrt},
    {"sin",    std::sin},
    {"cos",    std::cos},
    {"tan",    std::tan},
    {"arctan", std::atan},
    {"arcsin", std::asin},
    {"arccos", std::acos},
    {"cosh",   std::cosh},
    {"sinh",   std::sinh},
    {"tanh",   std::tanh},
    {"exp",    std::exp},
    {"ln",     std::log},
    {"log",    std::log},
    {"log10",  std::log10},
    {"log2",   std::log2},
    {"ceil",   std::ceil},
    {"floor",  std::floor},
};

std::map<std::string, BinaryOp> LangFunctions::binOperations =
{
    {"+", [](double a, double b){return a+b;}},
    {"-", [](double a, double b){return a-b;}},
    {"/", [](double a, double b){return a/b;}},
    {"*", [](double a, double b){return a*b;}},
    {"&", [](double a, double b){return a + b - sqrt(pow(a, 2) + pow(b, 2));}},
    {"|", [](double a, double b){return a + b + sqrt(pow(a, 2) + pow(b, 2));}},
    {"^", [](double a, double b){return pow(a, b);}},
};

std::map<std::string, UnaryOp> LangFunctions::unaryOperations =
{
    {"-", [](double a){return -a;}}
};


FunctionRef LangFunctions::FindFunction(std::string name)
{
    auto it = functions.find(name);
    return it == functions.end() ? nullptr: it->second;
}

std::string LangFunctions::FindFunction(FunctionRef func)
{
    for(auto& i: functions)
        if(i.second == func)
            return i.first;
    return "unknown";
}


BinaryOp LangFunctions::FindBinaryOp(std::string name)
{
    auto it = binOperations.find(name);
    return it == binOperations.end() ? nullptr: it->second;
}

std::string LangFunctions::FindBinaryOp(BinaryOp func)
{
    for(auto& i: binOperations)
        if(i.second == func)
            return i.first;
    return "unknown";
}

UnaryOp LangFunctions::FindUnaryOp(std::string name)
{
    auto it = unaryOperations.find(name);
    return it == unaryOperations.end() ? nullptr: it->second;
}

std::string LangFunctions::FindUnaryOp(UnaryOp func)
{
    for(auto& i: unaryOperations)
        if(i.second == func)
            return i.first;
    return "unknown";
}

std::string LangFunctions::FindFunctionCodeName(std::string name)
{
    if(name == "abs")
        return "fabs";
    if(name == "arctan")
        return "atan";
    if(name == "arcsin")
        return "asin";
    if(name == "arccos")
        return "acos";
    if(name == "ln" ||
            name == "log")
        return "log";
    return name;
}
