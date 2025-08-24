%{
#include "nodes.hpp"
#include "output.hpp"
#include <iostream>
#include <stdio.h>

extern int yylineno;
extern int yylex();
extern char *yytext;

extern int yychar;
extern YYSTYPE yylval;

void yyerror(const char* s) {
    output::errorSyn(yylineno); 
    exit(0);
}

std::shared_ptr<ast::Node> program;

using namespace std;
using namespace ast;
%}

%token RELOP TRUE FALSE WHILE IF BREAK CONTINUE ID STRING INT VOID BOOL BYTE NUM NUM_B
%token RBRACE LBRACE RBRACK LBRACK SC RETURN COMMA

%left OR
%left AND
%right ASSIGN
%left EQ NE
%nonassoc LT GT LE GE 
%left ADD SUB
%left MUL DIV
%right NOT
%left LPAREN RPAREN
%nonassoc ELSE
%left BINOP

%%

Program:
    Funcs {
        program = dynamic_pointer_cast<ast::Funcs>($1);
    }
;

Funcs:
      FuncDecl Funcs {
        auto list = dynamic_pointer_cast<ast::Funcs>($2);
        list->push_front(dynamic_pointer_cast<ast::FuncDecl>($1));
        $$ = list;
    }
    | /* empty */ {
        $$ = std::make_shared<ast::Funcs>();
    }
;

FuncDecl:
    RetType ID LPAREN Formals RPAREN LBRACE Statements RBRACE {
        auto id = dynamic_pointer_cast<ast::ID>($2);
        auto type = dynamic_pointer_cast<ast::PrimitiveType>($1);
        auto args = dynamic_pointer_cast<ast::Formals>($4);
        auto body = dynamic_pointer_cast<ast::Statements>($7);
        $$ = std::make_shared<ast::FuncDecl>(id, type, args, body);
    }
;

RetType:
      VOID {
        $$ = std::make_shared<ast::PrimitiveType>(BuiltInType::VOID);
    }
    | Type {
        $$ = dynamic_pointer_cast<ast::PrimitiveType>($1);
    }
;

Formals:
      FormalsList {
        $$ = dynamic_pointer_cast<ast::Formals>($1);
    }
    | /* empty */ {
        $$ = std::make_shared<ast::Formals>();
    }
;

FormalsList:
      FormalDecl COMMA FormalsList {
        auto tail = dynamic_pointer_cast<ast::Formals>($3);
        tail->push_front(dynamic_pointer_cast<ast::Formal>($1));
        $$ = tail;
    }
    | FormalDecl {
        $$ = std::make_shared<ast::Formals>();
        dynamic_pointer_cast<ast::Formals>($$)->push_back(dynamic_pointer_cast<ast::Formal>($1));
    }
;

FormalDecl:
    Type ID {
        auto id = dynamic_pointer_cast<ast::ID>($2);
        auto t = dynamic_pointer_cast<ast::PrimitiveType>($1);
        $$ = std::make_shared<ast::Formal>(id, t);
    }
;

Statements:
      Statements Statement {
        auto s = dynamic_pointer_cast<ast::Statements>($1);
        s->push_back(dynamic_pointer_cast<ast::Statement>($2));
        $$ = s;
    }
    | Statement {
        $$ = std::make_shared<ast::Statements>();
        dynamic_pointer_cast<ast::Statements>($$)->push_back(dynamic_pointer_cast<ast::Statement>($1));
    }
;

Statement:
      LBRACE Statements RBRACE {
        $$ = $2;
    }
    | Type ID ASSIGN Exp SC {
        auto rhs = dynamic_pointer_cast<ast::Exp>($4);
        auto lhs = dynamic_pointer_cast<ast::ID>($2);
        auto t = dynamic_pointer_cast<ast::PrimitiveType>($1);
        $$ = std::make_shared<ast::VarDecl>(lhs, t, rhs);
    }
    | Type ID SC {
        $$ = std::make_shared<ast::VarDecl>(
            dynamic_pointer_cast<ast::ID>($2),
            dynamic_pointer_cast<ast::PrimitiveType>($1));
    }
    | ID ASSIGN Exp SC {
        auto id = dynamic_pointer_cast<ast::ID>($1);
        $$ = std::make_shared<ast::Assign>(id, dynamic_pointer_cast<ast::Exp>($3));
    }
    | Call SC {
        $$ = $1;
    }
    | RETURN Exp SC {
        auto e = dynamic_pointer_cast<ast::Exp>($2);
        $$ = std::make_shared<ast::Return>(e);
    }
    | RETURN SC {
        auto result = std::make_shared<ast::Return>();
        $$ = result;
    }
    | IF LPAREN Exp RPAREN Statement ELSE Statement {
        auto cond = dynamic_pointer_cast<ast::Exp>($3);
        auto thenB = dynamic_pointer_cast<ast::Statement>($5);
        auto elseB = dynamic_pointer_cast<ast::Statement>($7);
        $$ = std::make_shared<ast::If>(cond, thenB, elseB);
    }
    | IF LPAREN Exp RPAREN Statement {
        $$ = std::make_shared<ast::If>(dynamic_pointer_cast<ast::Exp>($3), dynamic_pointer_cast<ast::Statement>($5));
    }
    | WHILE LPAREN Exp RPAREN Statement {
        auto cond = dynamic_pointer_cast<ast::Exp>($3);
        auto body = dynamic_pointer_cast<ast::Statement>($5);
        $$ = std::make_shared<ast::While>(cond, body);
    }
    | BREAK SC {
        $$ = std::make_shared<ast::Break>();
    }
    | CONTINUE SC {
        $$ = std::make_shared<ast::Continue>();
    }
    | ID LBRACK Exp RBRACK ASSIGN Exp SC {
        $$ = std::make_shared<ast::ArrayAssign>(
            dynamic_pointer_cast<ast::ID>($1),
            dynamic_pointer_cast<ast::Exp>($6),
            dynamic_pointer_cast<ast::Exp>($3));
    }
    | Type ID LBRACK Exp RBRACK SC {
        auto base = dynamic_pointer_cast<ast::PrimitiveType>($1);
        auto len = dynamic_pointer_cast<ast::Exp>($4);
        $$ = std::make_shared<ast::VarDecl>(
            dynamic_pointer_cast<ast::ID>($2),
            std::make_shared<ast::ArrayType>(base->type, len));
    }
;

Call:
      ID LPAREN ExpList RPAREN {
        $$ = std::make_shared<ast::Call>(
            dynamic_pointer_cast<ast::ID>($1),
            dynamic_pointer_cast<ast::ExpList>($3));
    }
    | ID LPAREN RPAREN {
        $$ = std::make_shared<ast::Call>(dynamic_pointer_cast<ast::ID>($1));
    }
;

ExpList:
      Exp COMMA ExpList {
        auto tail = dynamic_pointer_cast<ast::ExpList>($3);
        tail->push_front(dynamic_pointer_cast<ast::Exp>($1));
        $$ = tail;
    }
    | Exp {
        $$ = std::make_shared<ast::ExpList>();
        dynamic_pointer_cast<ast::ExpList>($$)->push_back(dynamic_pointer_cast<ast::Exp>($1));
    }
;

Type:
      BOOL {
        $$ = std::make_shared<ast::PrimitiveType>(BuiltInType::BOOL);
    }
    | INT {
        $$ = std::make_shared<ast::PrimitiveType>(BuiltInType::INT);
    }
    | BYTE {
        $$ = std::make_shared<ast::PrimitiveType>(BuiltInType::BYTE);
    }
;

Exp:
      Exp SUB Exp {
        $$ = std::make_shared<ast::BinOp>(
            dynamic_pointer_cast<ast::Exp>($1),
            dynamic_pointer_cast<ast::Exp>($3),
            BinOpType::SUB);
    }
    | Exp ADD Exp {
        $$ = std::make_shared<ast::BinOp>(
            dynamic_pointer_cast<ast::Exp>($1),
            dynamic_pointer_cast<ast::Exp>($3),
            BinOpType::ADD);
    }
    | Exp MUL Exp {
        $$ = std::make_shared<ast::BinOp>(
            dynamic_pointer_cast<ast::Exp>($1),
            dynamic_pointer_cast<ast::Exp>($3),
            BinOpType::MUL);
    }
    | Exp DIV Exp {
        $$ = std::make_shared<ast::BinOp>(
            dynamic_pointer_cast<ast::Exp>($1),
            dynamic_pointer_cast<ast::Exp>($3),
            BinOpType::DIV);
    }
    | Exp AND Exp {
        $$ = std::make_shared<ast::And>(
            dynamic_pointer_cast<ast::Exp>($1),
            dynamic_pointer_cast<ast::Exp>($3));
    }
    | Exp OR Exp {
        $$ = std::make_shared<ast::Or>(
            dynamic_pointer_cast<ast::Exp>($1),
            dynamic_pointer_cast<ast::Exp>($3));
    }
    | Exp EQ Exp {
        $$ = std::make_shared<ast::RelOp>(
            dynamic_pointer_cast<ast::Exp>($1),
            dynamic_pointer_cast<ast::Exp>($3),
            RelOpType::EQ);
    }
    | Exp NE Exp {
        $$ = std::make_shared<ast::RelOp>(
            dynamic_pointer_cast<ast::Exp>($1),
            dynamic_pointer_cast<ast::Exp>($3),
            RelOpType::NE);
    }
    | Exp LT Exp {
        $$ = std::make_shared<ast::RelOp>(
            dynamic_pointer_cast<ast::Exp>($1),
            dynamic_pointer_cast<ast::Exp>($3),
            RelOpType::LT);
    }
    | Exp GT Exp {
        $$ = std::make_shared<ast::RelOp>(
            dynamic_pointer_cast<ast::Exp>($1),
            dynamic_pointer_cast<ast::Exp>($3),
            RelOpType::GT);
    }
    | Exp LE Exp {
        $$ = std::make_shared<ast::RelOp>(
            dynamic_pointer_cast<ast::Exp>($1),
            dynamic_pointer_cast<ast::Exp>($3),
            RelOpType::LE);
    }
    | Exp GE Exp {
        $$ = std::make_shared<ast::RelOp>(
            dynamic_pointer_cast<ast::Exp>($1),
            dynamic_pointer_cast<ast::Exp>($3),
            RelOpType::GE);
    }
    | NOT Exp {
        $$ = std::make_shared<ast::Not>(dynamic_pointer_cast<ast::Exp>($2));
    }
    | LPAREN Exp RPAREN {
        $$ = $2;
    }
    | LPAREN Type RPAREN Exp {
        $$ = std::make_shared<ast::Cast>(
            dynamic_pointer_cast<ast::Exp>($4),
            dynamic_pointer_cast<ast::PrimitiveType>($2));
    }
    | ID LBRACK Exp RBRACK {
        $$ = std::make_shared<ast::ArrayDereference>(
            dynamic_pointer_cast<ast::ID>($1),
            dynamic_pointer_cast<ast::Exp>($3));
    }
    | ID {
        $$ = dynamic_pointer_cast<ast::ID>($1);
    }
    | Call {
        $$ = $1;
    }
    | NUM {
        $$ = dynamic_pointer_cast<ast::Num>($1);
    }
    | NUM_B {
        $$ = dynamic_pointer_cast<ast::NumB>($1);
    }
    | STRING {
        $$ = dynamic_pointer_cast<ast::String>($1);
    }
    | TRUE {
        $$ = std::make_shared<ast::Bool>(true);
    }
    | FALSE {
        $$ = std::make_shared<ast::Bool>(false);
    }
;
