%{
#include "output.hpp"
#include "tokens.hpp"
#include <iostream>
using output::printToken;
%}

%option yylineno
%option noyywrap

digit        ([0-9])
letter       ([a-zA-Z])
whitespace   ([\t\n\r ])
id           ({letter}({letter}|{digit})*)
hexa 		 ([0-7]([0-9]|[A-F]|[a-f]))


%x STRINGMODE
%x SLASHMODE
%%
void          return VOID;
int           return INT;
byte          return BYTE;
bool          return BOOL;
and           return AND;
or            return OR;
not           return NOT;
true          return TRUE;
false         return FALSE;
return        return RETURN;
if            return IF;
else          return ELSE;
while         return WHILE;
continue      return CONTINUE;
break         return BREAK;

;             return SC;
,             return COMMA;
\(            return LPAREN;
\)            return RPAREN;
\{            return LBRACE;
\}            return RBRACE;
\[            return LBRACK;
\]            return RBRACK;
=             return ASSIGN;
{whitespace}    ;
(==|!=|<=|>=|<|>)   return RELOP;
(\+|\*|\/|\-)       return BINOP;

\/\/[^\n\r]*       return COMMENT;
{id}               return ID ;

0                    { return NUM; } // A single '0' is a NUM
[1-9]{digit}*          { return NUM; } // Numbers starting from 1-9
[1-9][0-9]*b             return NUM_B;
0b             return NUM_B;
(\")                                {BEGIN(STRINGMODE) ;  return STRINGSTART; }
<STRINGMODE>([\n\r\x03]) 			{output::errorUnclosedString();}
<STRINGMODE><<EOF>>				    {output::errorUnclosedString();}
<STRINGMODE>\\                                  {BEGIN(SLASHMODE);}
<STRINGMODE>[^\\"\n\r]+ 			            {return STRING;}
<STRINGMODE>\"				                    {BEGIN(INITIAL); return STREND;}
<SLASHMODE>n					                {BEGIN(STRINGMODE); return DROPLINE;}
<SLASHMODE>x[0-7][0-9A-Fa-f] {
    int num = std::strtol(yytext + 1, nullptr, 16); // Skip 'x' and parse hex
    if (!((num >= 0x20 && num <= 0x7E) || num == 0x09 || num == 0x0A || num == 0x0D)) {
        output::errorUndefinedEscape(yytext);
    }
    BEGIN(STRINGMODE);
    return HEXA;
}
<SLASHMODE>r					                {BEGIN(STRINGMODE); return CARRIAGE;}
<SLASHMODE>\\					                {BEGIN(STRINGMODE); return BACKSLASH;}
<SLASHMODE>0 {BEGIN(STRINGMODE); return ZERO; }
<SLASHMODE>\"					                    {BEGIN(STRINGMODE); return QUOTE;}
<SLASHMODE>t					                {BEGIN(STRINGMODE); return TAB;}
<STRINGMODE>[ ]+                                { return SPACE; }

<SLASHMODE>(x|x[^"]|x[^"][^"])	                {output::errorUndefinedEscape(yytext);}
<SLASHMODE>[^"]					            {output::errorUndefinedEscape(yytext);}




.                output::errorUnknownChar(yytext[0]);

%%