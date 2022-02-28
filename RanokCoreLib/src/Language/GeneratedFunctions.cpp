#include "GeneratedFunctions.h"

#include <sstream>


std::map<std::string, std::function<std::string(VariableExpr*, ArgumentExpr*)>> GeneratedFunctions::VoidFunctions =
{
};

std::map<std::string, std::function<std::string(VariableExpr*, ArgumentExpr*, ConstExpr*)>> GeneratedFunctions::OneArgFunctions =
{
};


std::map<std::string, std::function<std::string(VariableExpr*, ArgumentExpr*, ConstExpr*, ConstExpr*)>> GeneratedFunctions::TwoArgFunctions =
{
    {"cut", &GeneratedFunctions::Cut}
};

std::function<std::string (VariableExpr*, ArgumentExpr*)> GeneratedFunctions::FindVoid(const std::string &name)
{
    auto it = VoidFunctions.find(name);
    if (it == VoidFunctions.end())
        return nullptr;
    else
        return (*it).second;
}

std::function<std::string (VariableExpr*, ArgumentExpr*, ConstExpr*)> GeneratedFunctions::FindOne(const std::string &name)
{
    auto it = OneArgFunctions.find(name);
    if (it == OneArgFunctions.end())
        return nullptr;
    else
        return (*it).second;
}

std::function<std::string(VariableExpr*, ArgumentExpr*, ConstExpr*, ConstExpr*)> GeneratedFunctions::FindTwo(const std::string &name)
{
    auto it = TwoArgFunctions.find(name);
    if (it == TwoArgFunctions.end())
        return nullptr;
    else
        return (*it).second;
}

std::string GeneratedFunctions::Cut(VariableExpr* var, ArgumentExpr *o, ConstExpr* start, ConstExpr* end)
{
    /*
        var s1 = -(z - 0.5); // Верх
        var s2 = z +0.5; // Снизу
        RETURN W & s1 & s2;
    */

    std::stringstream stream;
    stream << var->name << " & ";
    stream << "(-" << o->name << " + " << end->GetValue() << ") & ";
    stream << "(" << o->name << " - " << start->GetValue() << ")";

    return stream.str();
}
