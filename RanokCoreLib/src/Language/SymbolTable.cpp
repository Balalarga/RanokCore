#include "SymbolTable.h"

SymbolTable::SymbolTable():
    m_lastAddedVariable(nullptr)
{

}

void SymbolTable::Add(Expression *expr)
{
    if(auto it = dynamic_cast<ConstExpr*>(expr))
    {
        int id = FindConst(expr->name);
        if(id == -1)
            m_constants.push_back(it);
    }
    else if(auto it = dynamic_cast<VariableExpr*>(expr))
    {
        int id = FindVar(expr->name);
        if(id == -1)
            m_variables.push_back(it);
    }
    else if(auto it = dynamic_cast<ArgumentExpr*>(expr))
    {
        int id = FindArg(expr->name);
        if(id == -1)
            m_arguments.push_back(it);
    }
}

void SymbolTable::ForceAdd(Expression* expr)
{
    if(auto it = dynamic_cast<ConstExpr*>(expr))
    {
        int id = FindConst(expr->name);
        if(id != -1)
            m_constants[id] = it;
        else
            m_constants.push_back(it);
    }
    else if(auto it = dynamic_cast<VariableExpr*>(expr))
    {
        int id = FindVar(expr->name);
        if(id != -1)
            m_variables[id] = it;
        else
            m_variables.push_back(it);
    }
    else if(auto it = dynamic_cast<ArgumentExpr*>(expr))
    {
        int id = FindArg(expr->name);
        if(id != -1)
            m_arguments[id] = it;
        else
            m_arguments.push_back(it);
    }
}

ArgumentExpr *SymbolTable::GetArgAt(size_t id) const
{
    if(id >= m_arguments.size())
        return nullptr;

    return m_arguments[id];
}

std::vector<ArgumentExpr*> SymbolTable::GetAllArgs() const
{
    return m_arguments;
}

Expression* SymbolTable::Get(const std::string &name) const
{
    int id = FindArg(name);
    if(id != -1)
        return m_arguments[id];
    id = FindConst(name);
    if(id != -1)
        return m_constants[id];
    id = FindVar(name);
    if(id != -1)
        return m_variables[id];
    return nullptr;
}

void SymbolTable::Merge(const SymbolTable &oth)
{
//    for(auto& i: oth.m_arguments)
//        m_arguments[i.first] = i.second;

//    for(auto& i: oth.m_variables)
//        m_variables[i.first] = i.second;

//    for(auto& i: oth.m_constants)
//        m_constants[i.first] = i.second;

//    m_lastAddedVariable = oth.m_lastAddedVariable;
}

void SymbolTable::Concat(const SymbolTable &oth)
{
//    for(auto& i: oth.m_arguments)
//    {
//        for(int o = 0; o < m_arguments; o++)
//            if(i.first == )
//        if(m_arguments.find(i.first) == m_arguments.end())
//            m_arguments[i.first] = i.second;
//    }

//    for(auto& i: oth.m_variables)
//    {
//        if(m_variables.find(i.first) == m_variables.end())
//        {
//            m_variables[i.first] = i.second;
//            m_lastAddedVariable = i.second;
//        }
//    }

//    for(auto& i: oth.m_constants)
//    {
//        if(m_constants.find(i.first) == m_constants.end())
//            m_constants[i.first] = i.second;
//    }
}

ConstExpr *SymbolTable::GetConst(const std::string &name) const
{
    int id = FindConst(name);
    if(id != -1)
        return m_constants[id];
    return nullptr;
}

std::vector<ConstExpr *> SymbolTable::GetAllConst() const
{
    return m_constants;
}

ArgumentExpr *SymbolTable::GetArgument(const std::string &name) const
{
    int id = FindArg(name);
    if(id != -1)
        return m_arguments[id];
    return nullptr;
}

VariableExpr *SymbolTable::GetVariable(const std::string &name) const
{
    int id = FindVar(name);
    if(id != -1)
        return m_variables[id];
    return nullptr;
}

std::vector<VariableExpr *> SymbolTable::GetAllVariables() const
{
    return m_variables;
}

VariableExpr *SymbolTable::GetLastVar() const
{
    return m_lastAddedVariable;
}

int SymbolTable::FindArg(std::string name) const
{
    for(int i = 0; i < m_arguments.size(); i++)
        if(m_arguments[i]->name == name)
            return i;
    return -1;
}

int SymbolTable::FindConst(std::string name) const
{
    for(int i = 0; i < m_constants.size(); i++)
        if(m_constants[i]->name == name)
            return i;
    return -1;
}

int SymbolTable::FindVar(std::string name) const
{
    for(int i = 0; i < m_variables.size(); i++)
        if(m_variables[i]->name == name)
            return i;
    return -1;
}

bool SymbolTable::HasName(const std::string &name) const
{
    return Get(name) != nullptr;
}
