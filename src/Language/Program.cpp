#include "Program.h"
#include <list>
#include <queue>
#include <sstream>

using namespace std;

Program::Program():
    resultNode(nullptr)
{
    map<string, double> stdConstant{
        {"pi", 3.14159265358979323846},
        {"e", 2.71828182845904523536},
    };
    for(const auto& i: stdConstant)
    {
        auto numberExpr = NodeCreator::Instance().Create<NumberExpr>(i.second);
        auto constExpr = NodeCreator::Instance().Create<ConstExpr>(i.first, numberExpr);
        m_symbolTable.Add(constExpr);
    }
}

double Program::Compute(std::map<std::string, double> args) const
{
    if(!resultNode)
        return 0;
    for(auto& i: args)
    {
        auto node = m_symbolTable.GetArgument(i.first);
        if(!node)
        {
            error += "Unknown argument " + i.first;
            return 0;
        }
        node->SetValue(i.second);
    }

    double res = resultNode->GetValue();
    resultNode->Reset();

    return res;
}

double Program::Compute(cl_double3 args) const
{
    if(!resultNode)
        return 0;
    if(auto item = m_symbolTable.GetArgAt(0))
        item->SetValue(args.x);

    if(auto item = m_symbolTable.GetArgAt(1))
        item->SetValue(args.y);

    if(auto item = m_symbolTable.GetArgAt(2))
        item->SetValue(args.z);

    double res = resultNode->GetValue();

    resultNode->Reset();

    return res;
}


void Program::PrintTreeDepth(int depth) const
{
    set<string> vars;
    PrintNode(resultNode, vars, 0, depth);
    cout<<endl;
}

SymbolTable &Program::GetSymbolTable()
{
    return m_symbolTable;
}

void GetVariableBody(stringstream& stream, Expression* v)
{
    stream<<"(";
    if(auto casted = dynamic_cast<UnaryExpr*>(v))
    {
        stream << v->name;
        GetVariableBody(stream, casted->expr);
    }
    else if(auto casted = dynamic_cast<BinaryExpr*>(v))
    {
        if(v->name == "^")
        {
            stream << "pow(";
            GetVariableBody(stream, casted->left);
            stream<<", ";
            GetVariableBody(stream, casted->right);
            stream << ")";
        }
        else if(v->name == "&")
        {
            stream << "__rand(";
            GetVariableBody(stream, casted->left);
            stream<<", ";
            GetVariableBody(stream, casted->right);
            stream << ")";
        }
        else if(v->name == "|")
        {
            stream << "__ror(";
            GetVariableBody(stream, casted->left);
            stream<<", ";
            GetVariableBody(stream, casted->right);
            stream << ")";
        }
        else
        {
            GetVariableBody(stream, casted->left);
            stream << v->name;
            GetVariableBody(stream, casted->right);
        }
    }
    else if(auto casted = dynamic_cast<FunctionExpr*>(v))
    {
        stream << LangFunctions::FindFunctionCodeName(casted->name)<<"(";
        GetVariableBody(stream, casted->arg);
        stream << ")";
    }
    else if(v)
    {
        stream << v->name;
    }
    stream<<")";
}

string Program::GetOpenclCode() const
{
    stringstream code;
    auto variables = m_symbolTable.GetAllVariables();
    auto constants = m_symbolTable.GetAllConst();
    auto args = m_symbolTable.GetAllArgs();
    code << "double __resultFunc(";
    for(int i = 0; i < args.size(); i++)
    {
        code << "double "<<args[i]->name;
        if(i != args.size()- 1)
            code<<", ";
    }
    code << ")\n{\n";
    for(auto& c: constants)
    {
        code << "const double "<<c->name <<" = "<< c->GetValue() << ";\n";
    }

    for(auto& v: variables)
    {
        code << "double "<<v->name <<" =";
        GetVariableBody(code, v->expr);
        code << ";\n";
    }

    code << "return ";
    GetVariableBody(code, resultNode);
    code << ";\n}\n";
    return code.str();
}

void Program::PrintNode(Expression *node, set<string>& vars, int currDepth, int maxDepth) const
{
    if(maxDepth == currDepth)
        return;

    cout<<'\n';

    for(int i = 0; i < currDepth; i++)
        cout<<" |";

    if(auto casted = dynamic_cast<NumberExpr*>(node))
    {
        cout<<"NumberExpr: "<<casted->GetValue();
    }
    else if(auto casted = dynamic_cast<ConstExpr*>(node))
    {
        cout<<"ConstExpr: "<<casted->name<<" = "<<casted->GetValue();
    }
    else if(auto casted = dynamic_cast<ArgumentExpr*>(node))
    {
        cout<<"ArgumentExpr: "<<casted->name;
    }
    else if(auto casted = dynamic_cast<VariableExpr*>(node))
    {
        cout<<"VariableExpr: "<<casted->name;
        if(vars.find(casted->name) == vars.end())
        {
            vars.insert(casted->name);
            PrintNode(casted->expr, vars, currDepth+1, maxDepth);
        }
    }
    else if(auto casted = dynamic_cast<UnaryExpr*>(node))
    {
        cout<<"UnaryExpr: "<<casted->op;
        PrintNode(casted->expr, vars, currDepth+1, maxDepth);
    }
    else if(auto casted = dynamic_cast<BinaryExpr*>(node))
    {
        cout<<"BinaryExpr: "<<casted->op;
        PrintNode(casted->left, vars, currDepth+1, maxDepth);
        PrintNode(casted->right, vars, currDepth+1, maxDepth);
    }
    else if(auto casted = dynamic_cast<FunctionExpr*>(node))
    {
        cout<<"FunctionExpr: "<<LangFunctions::FindFunction(casted->func);
        PrintNode(casted->arg, vars, currDepth+1, maxDepth);
    }
}
string Program::GetError() const
{
    return error;
}

bool Program::IsError() const
{
    return !error.empty();
}

void Program::SetResult(Expression *expr)
{
    resultNode = expr;
}

Expression* Program::MergeProgram(const Program *program)
{
    m_symbolTable.Merge(program->m_symbolTable);
    if(program->resultNode)
        return program->resultNode;
    else
        return m_symbolTable.GetLastVar();
}
