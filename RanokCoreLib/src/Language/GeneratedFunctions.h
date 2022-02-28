#ifndef GENERATEDFUNCTIONS_H
#define GENERATEDFUNCTIONS_H

#include "Node.h"
#include <functional>
#include <map>


class GeneratedFunctions
{
public:
    GeneratedFunctions() = delete;

    static std::map<std::string, std::function<std::string(VariableExpr*, ArgumentExpr*)>> VoidFunctions;
    static std::function<std::string(VariableExpr*, ArgumentExpr*)> FindVoid(const std::string& name);

    static std::map<std::string, std::function<std::string(VariableExpr*, ArgumentExpr*, ConstExpr*)>> OneArgFunctions;
    static std::function<std::string(VariableExpr*, ArgumentExpr*, ConstExpr*)> FindOne(const std::string& name);

    static std::map<std::string, std::function<std::string(VariableExpr*, ArgumentExpr*, ConstExpr*, ConstExpr*)>> TwoArgFunctions;
    static std::function<std::string(VariableExpr*, ArgumentExpr*, ConstExpr*, ConstExpr*)> FindTwo(const std::string& name);

    static std::string Cut(VariableExpr* var, ArgumentExpr* o, ConstExpr* start, ConstExpr* end);

};

#endif // GENERATEDFUNCTIONS_H
