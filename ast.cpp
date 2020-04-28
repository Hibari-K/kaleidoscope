#include<string>
#include<vector>

// ExprAST - Base class for all expression nodes
class ExprAST{

public:
    virtual ~ExprAST(){}
};

// NumberExprAST = Expression class for numneric literals like "1.0"
class NumberExprAST : public ExprAST{
    double Val;

public:
    NumberExprAST(double Val) : Val(Val){}
};


// VariableExprAST - Expression class for  referencing a variable, like "a"
class VariableExprAST: public ExprAST{
    std::string Name;

public:
    VariableExprAST(const std::string &Name) : Name(Name){}
};

// BinaryExprAST = Expression class for a binary operator
class BinaryExprAST : public ExprAST{

    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;

public:
    BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST>)
    : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};


// CallExprAST = Expression class for function calls
class CallExprAST : public ExprAST{

    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;

public:
    CallExprAST(const std::string &Callee, std::vector<std::unique_ptr<ExprAST>> Args)
    : Callee(Callee), Args(std::move(Args)){}
};


class PrototypeAST{
    std::string Name;
    std::vector<std::string> Args;

public:
    PrototypeAST(const std::string &name, std::vector<std::string> Args)
    : Name(name), Args(std::move(Args)){}

    const std::string &getName() const{ return Name;}
};

// FunctionAST - This class represents a function definition itself
class FunctionAST{
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<ExprAST> Body)
    : Proto(std::move(Proto)), Body(std::move(Body)) {}
};

