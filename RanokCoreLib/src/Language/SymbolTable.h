#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "Node.h"
#include <vector>
#include <unordered_map>

class SymbolTable
{
public:
    SymbolTable();

    void Add(Expression *expr);
    void ForceAdd(Expression* expr);

    ArgumentExpr* GetArgAt(size_t id) const;
    std::vector<ArgumentExpr *> GetAllArgs() const;
    ArgumentExpr* GetArgument(const std::string& name) const;

    Expression *Get(const std::string& name) const;

    void Merge(const SymbolTable& oth);
    void Concat(const SymbolTable& oth);

    ConstExpr* GetConst(const std::string& name) const;
    std::vector<ConstExpr*> GetAllConst() const;
    VariableExpr* GetVariable(const std::string& name) const;
    std::vector<VariableExpr*> GetAllVariables() const;
    VariableExpr* GetLastVar() const;


private:
    std::vector<ArgumentExpr*> m_arguments;
    std::vector<VariableExpr*> m_variables;
    std::vector<ConstExpr*> m_constants;
    VariableExpr* m_lastAddedVariable;

    int FindArg(std::string name) const;
    int FindConst(std::string name) const;
    int FindVar(std::string name) const;

    bool HasName(const std::string& name) const;
};

#endif // SYMBOLTABLE_H
