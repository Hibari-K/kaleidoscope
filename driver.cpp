#include<string>
#include"kaleido.h"


static void HandleDefinition(){

    if(ParseDefinition()){
        fprintf(stderr, "Parsed a function definition.\n");
    }
    else
        getNextToken(); // skip token for error recovery
}

static void HandleExtern(){
    if(ParseExtern())
        fprintf(stderr, "Parsed an extern\n");
    else
        getNextToken(); // skip
}

static void HandleTopLevelExpression(){

    //Evaluate a top-level expression into an anonymous function
    if(ParseTopLevelExpr()){
        fprintf(stderr, "Parsed a top-level expr\n");
    }
    else{
        getNextToken(); //skip
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

    // run the main loop
    MainLoop();

    return 0;
}