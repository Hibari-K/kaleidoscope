#include<string>
#include"kaleido.h"

using namespace llvm;

static void HandleDefinition(){

    /*
    if(ParseDefinition()){
        fprintf(stderr, "Parsed a function definition.\n");
    }
    else
        getNextToken(); // skip token for error recovery
    */

   if(auto FnAST = ParseDefinition()){
       if(auto *FnIR = FnAST->codegen()){
           fprintf(stderr, "Read function definition: \n");
           FnIR->print(errs());
           fprintf(stderr, "\n");
       }
       else{
           // skip token for error recovery
           getNextToken();
       }
   }
}

static void HandleExtern(){
    /*
    if(ParseExtern())
        fprintf(stderr, "Parsed an extern\n");
    else
        getNextToken(); // skip
    */

   if(auto ProtoAST = ParseExtern()){
       if(auto *FnIR = ProtoAST->codegen()){
           fprintf(stderr, "Read extern: \n");
           FnIR->print(errs());
           fprintf(stderr, "\n");
       }
   }
   else{
       // skip
       getNextToken();
   }
}

static void HandleTopLevelExpression(){

    /*
    //Evaluate a top-level expression into an anonymous function
    if(ParseTopLevelExpr()){
        fprintf(stderr, "Parsed a top-level expr\n");
    }
    else{
        getNextToken(); //skip
    }
    */

   if(auto FnAST = ParseTopLevelExpr()){
       if(auto *FnIR = FnAST->codegen()){
           fprintf(stderr, "Read top-level expression: \n");
           FnIR->print(errs());
           fprintf(stderr, "\n");
       }
       else{
           //skip
           getNextToken();
       }
   }
}


static void MainLoop(){

    while(1){
        
        fprintf(stderr, ">>> "); 

        switch(CurTok){
            case tok_eof:
                return;
            case ';': // ignore top-level semicolons
                getNextToken(); //eat ;
                break;
            case tok_def:
                HandleDefinition();
                break;
            case tok_extern:
                HandleExtern();
                break;
            default:
                HandleTopLevelExpression();
                break;
        }
        
       
    }
}


int main(){

    // Install standard binary operators
    // 1 is lowest precedence
    BinopPrecedence['<'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 20;
    BinopPrecedence['*'] = 40; // highest
    BinopPrecedence['/'] = 40; // highest

    // prime the first token
    fprintf(stderr, ">>> ");
    getNextToken();

    TheModule = std::make_unique<Module>("Test Make JIT", TheContext);

    // run the main loop
    MainLoop();

    TheModule->print(errs(), nullptr);

    return 0;
}