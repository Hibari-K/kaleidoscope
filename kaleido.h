#include<string>
#include<vector>
#include<map>


enum Token{
    tok_eof = -1,

    // commands
    tok_def = -2,
    tok_extern = -3,
    
    // prtimary
    tok_identifier = -4,
    tok_number = -5,

};


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






extern std::string IdentifierStr;   // Filled in if tok_identifier
extern double NumVal;
extern int CurTok;
extern std::map<char, int> BinopPrecedence;

int gettok();


std::unique_ptr<ExprAST> ParseExpression();
std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS);

int getNextToken();

// LogError - little helper functions for error handling
std::unique_ptr<ExprAST> LogError(const char *Str);
std::unique_ptr<PrototypeAST> LogErrorP(const char*Str);

//numberexpr ::= number
std::unique_ptr<ExprAST> ParseNumberExpr();

//parenexpr ::= '(' expression ')'
std::unique_ptr<ExprAST> ParseParenExpr();

// identifierexpr   ::= identifier
//                  ::= identifier '(' expression ')'
std::unique_ptr<ExprAST> ParseIdentifierExpr();

// primary  ::= identifierexpr
//          ::= numberexpr
//          ::= parenexpr
std::unique_ptr<ExprAST> ParsePrimary();

//BinopPrecedence - This holds the precedence for each binary operator that is defined
//std::map<char, int> BinopPrecedence;

// GetTokPrecedence - Get the precedence of the pending binary operator token
int GetTokPrecedence();
// expression ::= primary binoprhs
std::unique_ptr<ExprAST> ParseExpression();
// binoprhs
std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS);
// prototype ::= id '(' id* ')'
std::unique_ptr<PrototypeAST> ParsePrototype();
// definition ::= 'def' prototype expression
std::unique_ptr<FunctionAST> ParseDefinition();
// external ::= 'extern' prototype
std::unique_ptr<PrototypeAST> ParseExtern();
// toplevelexpr ::= expression
std::unique_ptr<FunctionAST> ParseTopLevelExpr();



