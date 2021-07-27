#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <cmath>

#include "LangFunctions.h"

// Здесь описаны узлы для построения абстрактного синтаксического дерева

struct Expression
{
    friend class NodeCreator;
public:
    virtual ~Expression(){}
    virtual double GetValue() = 0;
    virtual void Reset(){};
    bool IsError()
    {
        return !error.empty();
    }
    inline void SetValue(double value)
    {
        store = value;
        ready = true;
    }
    std::string error = "";
    std::string name = "";

protected:
    Expression(){}
    void operator delete(void * ptr) {
        std::free(ptr);
    }
    bool ready = false;
    double store = 0;
};


struct NumberExpr: public Expression
{
    friend class NodeCreator;
public:
    virtual double GetValue() override
    {
        return store;
    };

protected:
    NumberExpr(double value)
    {
        ready = true;
        store = value;
        name = std::to_string(value);
    }
    using Expression::operator delete;
};


struct ConstExpr: public Expression
{
    friend class NodeCreator;
public:
    virtual double GetValue() override
    {
        if(!ready)
            SetValue(expr->GetValue());
        return store;
    };

    Expression* expr;

protected:
    ConstExpr(std::string name, Expression* expr):
        expr(expr)
    {
        ready = false;
        this->name = name;
    }
    using Expression::operator delete;
};

struct ArgumentExpr: public Expression
{
    friend class NodeCreator;
public:
    virtual double GetValue() override
    {
        return store;
    };

    std::pair<double, double> limits;

protected:
    ArgumentExpr(std::string name, std::pair<double, double> lims):
        limits(lims)
    {
        this->name = name;
    }
    using Expression::operator delete;
};

struct VariableExpr: public Expression
{
    friend class NodeCreator;
public:
    virtual double GetValue() override
    {
        if(!ready)
            SetValue(expr->GetValue());
        return store;
    };
    virtual void Reset() override
    {
        ready = false;
        expr->Reset();
    }

    Expression* expr;

protected:
    VariableExpr(std::string name, Expression* expr):
        expr(expr)
    {
        this->name = name;
    }
    using Expression::operator delete;
};

struct UnaryExpr: public Expression
{
    friend class NodeCreator;
public:
    virtual double GetValue() override
    {
        if(!ready)
        {
            if(op)
                SetValue(op(expr->GetValue()));
            else
                error = "Unknown operation: "+name;
        }
        return store;
    };
    virtual void Reset() override
    {
        ready = false;
        expr->Reset();
    }

    UnaryOp op;
    Expression* expr;

protected:
    UnaryExpr(std::string opName, Expression* expr):
        op(LangFunctions::FindUnaryOp(opName)),
        expr(expr)
    {
        name = opName;
    }
    using Expression::operator delete;
};

struct BinaryExpr: public Expression
{

    friend class NodeCreator;
public:
    virtual double GetValue() override
    {
        if(!ready)
        {
            if(op)
                SetValue(op(left->GetValue(), right->GetValue()));
            else
                error = "Unknown binary operation "+name;
        }
        return store;
    };
    virtual void Reset() override
    {
        ready = false;
        left->Reset();
        right->Reset();
    }

    BinaryOp op;
    Expression* left, *right;

protected:
    BinaryExpr(std::string opName, Expression* left,
               Expression* right):
        op(LangFunctions::FindBinaryOp(opName)),
        left(left),
        right(right)
    {
        name = opName;
    }
    using Expression::operator delete;
};

struct FunctionExpr: public Expression
{
    friend class NodeCreator;
public:
    virtual double GetValue() override
    {
        if(!ready)
            SetValue(func(arg->GetValue()));
        return store;
    };
    virtual void Reset() override
    {
        ready = false;
        arg->Reset();
    }

    FunctionRef func;
    Expression* arg;

protected:
    FunctionExpr(FunctionRef func, Expression* arg):
        func(func),
        arg(arg)
    {
        name = LangFunctions::FindFunction(func);
    }
    using Expression::operator delete;
};

#endif // NODE_H
