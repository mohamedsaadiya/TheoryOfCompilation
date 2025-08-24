%{
/* Declarations section */ 
#include <stdio.h>
#include "./output.hpp"
#include "./parser.tab.h"
#include "nodes.hpp"
%}

%option yylineno 
%option noyywrap 

letters                           ([a-zA-Z]) 
digit                            ([0-9]) 
whitespace                        ([\t\n\r ])
%% 

void                               return VOID; 
int                                return INT; 
byte                               return BYTE; 
bool                               return BOOL; 
and                                return AND; 
or                                 return OR; 
not                                return NOT; 
true                               return TRUE; 
false                              return FALSE; 
return                             return RETURN; 
if                                 return IF; 
else                               return ELSE; 
while                              return WHILE; 
break                              return BREAK; 
continue                           return CONTINUE; 
{whitespace}*				 	    ;
(;)                                  return SC; 
(,)                                  return COMMA; 
\(                                   return LPAREN; 
\)                                   return RPAREN; 
\{                                   return LBRACE; 
\}                                   return RBRACE; 
\[                                   return LBRACK; 
\]                                   return RBRACK;    
\=                                   return ASSIGN; 
"=="                                 return EQ; 
"!="                                 return NE; 
"<="                                 return LE; 
">="                                 return GE; 
"<"                                  return LT; 
">"                                  return GT; 
\+                                   return ADD; 
\*                                  return MUL; 
\/                                  return DIV; 
\-                                   return SUB; 
{letters}({letters}|{digit})*                                { yylval = std::make_shared<ast::ID>(yytext); return ID; }
[1-9]{digit}*|0                                              { yylval = std::make_shared<ast::Num>(yytext); return NUM; }
[1-9]{digit}*b|0b                                            { yylval = std::make_shared<ast::NumB>(yytext); return NUM_B; }
\"([^\n\r\"\\]|\\[rnt"\\])+\"                                {yylval = std::make_shared<ast::String>(yytext); return STRING;}
(\/\/)[^\n\r]*                                               {;}
.                                                            {output::errorLex(yylineno);exit(0);}

%%
