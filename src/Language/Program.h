#ifndef PROGRAM_H
#define PROGRAM_H

#include <map>
#include <set>
#include "SymbolTable.h"
#include "NodeCreator.h"
#include <CL/cl.h>


// Класс, содержащий информацию о абстрактном синтаксическом дереве
// и его аргументах, константах, и переменных

class Program
{
public:
    Program();

    double Compute(std::map<std::string, double> arguments) const;
    double Compute(cl_double3 args) const;

    std::string GetError() const;
    bool IsError() const;

    void SetResult(Expression* expr);

    Expression *MergeProgram(const Program* program);

    void PrintTreeDepth(int depth) const;
    SymbolTable& GetSymbolTable();

    std::string GetOpenclCode() const;

private:
    Expression* resultNode;
    SymbolTable m_symbolTable;

    mutable std::string error = "";

    void PrintNode(Expression* node, std::set<std::string>& vars,
                   int currDepth, int maxDepth) const;
};

#endif // PROGRAM_H
