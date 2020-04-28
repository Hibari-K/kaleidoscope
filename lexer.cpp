#include<iostream>
#include<string>
#include"kaleido.h"

std::string IdentifierStr;   // Filled in if tok_identifier
double NumVal;               // Filled in if tok_number

int gettok(){
    static int LastChar = ' ';

    //Skip any whitespace
    while(isspace(LastChar))
        LastChar = getchar();

    // isalpha check if LastChar is alphabetic character
    if (isalpha(LastChar)){ // identifier: [a-zA-Z][a-zA-Z0-9]*
        IdentifierStr = LastChar;

        // isalnum check if LastChar is either a decimal digit or an letter
        while(isalnum((LastChar = getchar())))
            IdentifierStr += LastChar;
        
        if(IdentifierStr == "def")
            return tok_def;
        if(IdentifierStr == "extern")
            return tok_extern;
        
        return tok_identifier;
    }

    // check if LastChar is double number
    if(isdigit(LastChar) || LastChar == '.'){ // Number: [0-9.]+
        std::string NumStr;

        do{
            NumStr += LastChar;
            LastChar = getchar();
        }while(isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), 0);
        return tok_number;
    }

    // comment until enf of line
    if(LastChar == '#'){
        do{
            LastChar = getchar();
        }while(LastChar != EOF && LastChar != '\n' && LastChar == '\r');

        // if '\n' or '\r'
        if(LastChar != EOF)
            return gettok();
    }

    // Check for end of file. Don't eat the EOF
    if(LastChar == EOF)
        return tok_eof;
    
    // Otherwise, just return the character as its ascii value
    // e.g., '+'
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}