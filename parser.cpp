#include<string>
#include<vector>
#include<map>
#include"kaleido.h"

//static std::unique_ptr<ExprAST> ParseExpression();
//static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS);

int CurTok;
std::map<char, int> BinopPrecedence;

//static int CurTok;
int getNextToken(){
    return CurTok = gettok();
}

// LogError - little helper functions for error handling
std::unique_ptr<ExprAST> LogError(const char *Str){
    fprintf(stderr, "LogError: %s\n", Str);
    return nullptr;
}

std::unique_ptr<PrototypeAST> LogErrorP(const char*Str){
    LogError(Str);
    return nullptr;
}


//numberexpr ::= number
std::unique_ptr<ExprAST> ParseNumberExpr(){
    auto Result = std::make_unique<NumberExprAST>(NumVal);
    getNextToken();
    return std::move(Result);
}


//parenexpr ::= '(' expression ')'
std::unique_ptr<ExprAST> ParseParenExpr(){
    getNextToken();
    
    auto V = ParseExpression();
    if(!V)
        return nullptr;
    
    if(CurTok != ')')
        return LogError("expected ')");
    
    getNextToken();

    return V;
}


// identifierexpr   ::= identifier
//                  ::= identifier '(' expression ')'
std::unique_ptr<ExprAST> ParseIdentifierExpr(){
    std::string IdName = IdentifierStr;

    getNextToken();

    if(CurTok != '(')
        return std::make_unique<VariableExprAST>(IdName);
    
    //Call
    getNextToken(); //eat'('
    std::vector<std::unique_ptr<ExprAST>> Args;
    if(CurTok != ')'){

        while(1){
            if(auto Arg = ParseExpression())
                Args.push_back(std::move(Arg));
            else
                return nullptr;
            
            if(CurTok == ')')
                break;
            
            if(CurTok != ',')
                return LogError("Expected ')' or ',' in argument list.");
            
            getNextToken();
            
        }
    }

    getNextToken(); // eat ')'

    return std::make_unique<CallExprAST>(IdName, std::move(Args));
}


// primary  ::= identifierexpr
//          ::= numberexpr
//          ::= parenexpr
std::unique_ptr<ExprAST> ParsePrimary(){

    switch(CurTok){
        default:
            return LogError("unknown token when expecting an expression");
        case tok_identifier:
            return ParseIdentifierExpr();
        case tok_number:
            return ParseNumberExpr();
        case '(':
            return ParseParenExpr();
    }
}


// GetTokPrecedence - Get the precedence of the pending binary operator token
int GetTokPrecedence(){
    if(!isascii(CurTok))
        return -1;
    
    // Make sure it's a declared binop
    int TokPrec = BinopPrecedence[CurTok];
    if(TokPrec <= 0) return -1;
    return TokPrec;
}

// expression ::= primary binoprhs
std::unique_ptr<ExprAST> ParseExpression(){
    auto LHS = ParsePrimary();
    if(!LHS)
        return nullptr;
    
    return ParseBinOpRHS(0, std::move(LHS));
}

// binoprhs
std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS)
{

    // If this is a binop, find its precedence
    while(1){
        int TokPrec = GetTokPrecedence();

        // If this is a binop that binds at least as tightly as the currennt binop,
        // consume it, otherwise we are done.
        if(TokPrec < ExprPrec)
            return LHS;

        // we know this is a binop
        int BinOp = CurTok;
        getNextToken(); // eat binop

        // Parse the primary expression after the binary operator
        auto RHS = ParsePrimary();
        if(!RHS)
            return nullptr;


        // If BinOp binds less tightly with RHS than the operator after RHS,
        // let the pending operator take RHS as its LHS        
        int NextPrec = GetTokPrecedence();
        if(TokPrec < NextPrec){
            // left is more precedence
            RHS = ParseBinOpRHS(TokPrec+1, std::move(RHS));
            if(!RHS)
                return nullptr;
        }

        // Merge LHS/RHS
        LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
    }
}


// prototype ::= id '(' id* ')'
std::unique_ptr<PrototypeAST> ParsePrototype(){

    if(CurTok != tok_identifier)
        return LogErrorP("Expected function name in prototype");
    
    std::string FnName = IdentifierStr;
    getNextToken();

    if(CurTok != '(')
        return LogErrorP("Expected '(' in prototype");
    
    // Read the list of argument names
    std::vector<std::string> ArgNames;
    while(getNextToken() == tok_identifier)
        ArgNames.push_back(IdentifierStr);
    
    if(CurTok != ')')
        return LogErrorP("Expected ')' in prototype");
    
    // success
    getNextToken(); //eat ')'

    return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

// definition ::= 'def' prototype expression
std::unique_ptr<FunctionAST> ParseDefinition(){

    getNextToken();
    auto Proto = ParsePrototype();
    if(!Proto)
        return nullptr;
    
    if(auto E = ParseExpression())
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    
    return nullptr;
}

// external ::= 'extern' prototype
std::unique_ptr<PrototypeAST> ParseExtern(){
    getNextToken(); // eat extern
    return ParsePrototype();
}

// toplevelexpr ::= expression
std::unique_ptr<FunctionAST> ParseTopLevelExpr(){

    if(auto E = ParseExpression()){

        // make an anonymous proto
        auto Proto= std::make_unique<PrototypeAST>("", std::vector<std::string>());
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));

        return nullptr;
    }

    return nullptr;
}





