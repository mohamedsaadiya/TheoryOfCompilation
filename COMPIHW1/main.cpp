#include "tokens.hpp"
#include "output.hpp"
#include <string>
#include <iostream>

static const std::string token_names[] = {
    "__FILLER_FOR_ZERO",
    "VOID",
    "INT",
    "BYTE",
    "BOOL",
    "AND",
    "OR",
    "NOT",
    "TRUE",
    "FALSE",
    "RETURN",
    "IF",
    "ELSE",
    "WHILE",
    "BREAK",
    "CONTINUE",
    "SC",
    "COMMA",
    "LPAREN",
    "RPAREN",
    "LBRACE",
    "RBRACE",
    "LBRACK",
    "RBRACK",
    "ASSIGN",
    "RELOP",
    "BINOP",
    "COMMENT",
    "ID",
    "NUM",
    "NUM_B",
    "STRING"
};

void createCorrect ();
int main() {
    enum tokentype token;

    // read tokens until the end of file is reached
    while ((token = static_cast<tokentype>(yylex()))) {
        if (token == STRINGSTART){
            createCorrect();
        }
        else {
     output::printToken(yylineno,token,yytext);
    }
}
    return 0;
}

void createCorrect (){
    std::string curr;
    enum tokentype token = static_cast<tokentype>(yylex());
    int x=1;
     x+=1;
    
     
    while (token != STREND){
        
        if(token == ZERO){
            while (token != STREND){
                
                token = static_cast<tokentype>(yylex());

        }
        break;
    }
        else if (token == HEXA) {
            std::string hexString = std::string( yytext).substr(1);
            char c = static_cast<char>(std::strtol(hexString.c_str(), nullptr, 16));
            curr += c;
        ;
        }
        else if (token == DROPLINE){
            curr += "\n";
        }
        else if (token == CARRIAGE){
            curr += "\r";
        }
        else if (token == BACKSLASH){
            curr += "\\";
        }
        else if (token == QUOTE){
            curr += "\"";
        }
        else if (token == STRING){
           
                curr += yytext;    
        }
        else if (token ==TAB){
            curr += "\t";
        }
        else if (token == SPACE){
            curr += " ";
        }
        else if (token == TEXT){
            curr += yytext;
        }
        else if (token == ID){
            curr += yytext;
        }
        else if (token == NUM){
            curr += yytext;
        }
        else if (token == NUM_B){
            curr += yytext;
        } 
        else if (token == QUOTE){
            curr += "\"";
        }  
      
        else { 
            curr+= token_names[token];
}        
token = static_cast<tokentype>(yylex());
    }
   
    std::cout <<yylineno << " STRING " << curr << std::endl;  
}



