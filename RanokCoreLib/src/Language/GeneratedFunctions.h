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

    static std::map<std::string, std::function<std::string(VariableExpr*, ArgumentExpr*, double)>> OneArgFunctions;
    static std::function<std::string(VariableExpr*, ArgumentExpr*, double)> FindOne(const std::string& name);

    static std::map<std::string, std::function<std::string(VariableExpr*, ArgumentExpr*, double, double)>> TwoArgFunctions;
    static std::function<std::string(VariableExpr*, ArgumentExpr*, double, double)> FindTwo(const std::string& name);

    static std::string Cut(VariableExpr* var, ArgumentExpr* o, double start, double end);

};

#endif // GENERATEDFUNCTIONS_H
