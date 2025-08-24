#ifndef SEMANTICVISITOR_H
#define SEMANTICVISITOR_H


#include "visitor.hpp"
#include "output.hpp"
#include <vector>
#include <iostream>
#include "symbolTable.hpp"
#include <cstring>
#include<string>


using namespace std;



class Scope{
    public:
        SymbolTable symbolTable; // Symbol table for the current scope
        vector<symbol> declaredFuncs; 
        int offset; 
        string retType; // Type of the return value for functions in this scope
        string type;
        int numOfVariables;
        int nextVariableOffset; // Number of variables in this scope

        Scope(string type,int nextVariableOffset, string retType = "")
            : type(type),nextVariableOffset(nextVariableOffset), retType(retType), numOfVariables(0) {}
        
        // Check if a variable exists in the symbol table by name
    bool variableExists(std::string varName) const {
        for (int i = 0; i < symbolTable.symbols.size(); ++i) {
            const symbol& sym = symbolTable.symbols[i];
            if (sym.name == varName && !sym.isFunction) {
                return true;
            }
        }
        return false;
    }

    // Check if a function exists in declaredFuncs by name (parameter is string, not reference)
    bool functionExists(std::string funcName) const {
        for (int i = 0; i < declaredFuncs.size(); ++i) {
            const symbol& sym = declaredFuncs[i];
            if (sym.name == funcName && sym.isFunction) {
                return true;
            }
        }
        return false;
    }
};

class semanticVisitor : public Visitor {
public:
    std::vector<Scope> scopes; 
    output::ScopePrinter printer;
    int sCount;
    vector<ast::FuncDecl> funcDecl;
    semanticVisitor();
    void enterScope(string type,int nextOffset);
    void addBuiltInFunction(const std::string &name, ast::BuiltInType paramType);
    bool isNumber(string type);
    void exitScope();
    void emptyScope();
    std::string getTypeAsString(std::shared_ptr<ast::Type> type);
    std::string getTypeAsUpperString(std::shared_ptr<ast::Type> type);
    string toUpperString(ast::BuiltInType type);
    string toLowerString(ast::BuiltInType type);
    void visit(ast::Num &node) override;//
    void visit(ast::NumB &node) override;//
    void visit(ast::String &node) override;//
    void visit(ast::Bool &node) override;//
    void visit(ast::ID &node) override;//
    void visit(ast::BinOp &node) override;//
    void visit(ast::RelOp &node) override;//
    void visit(ast::Not &node) override;//
    void visit(ast::And &node) override;//
    void visit(ast::Or &node) override;//
   void visit(ast::ArrayType &node) override;//
   void visit(ast::PrimitiveType &node) override;//
   void visit(ast::ArrayDereference &node) override;//
   void visit(ast::ArrayAssign &node) override;//
    void visit(ast::Cast &node) override;//
    void visit(ast::ExpList &node) override;//
    void visit(ast::Call &node) override;//
    void visit(ast::Statements &node) override;//
    void visit(ast::Break &node) override;//
    void visit(ast::Continue &node) override;//
    void visit(ast::Return &node) override;//
    void visit(ast::If &node) override;//
    void visit(ast::While &node) override;//
    void visit(ast::VarDecl &node) override;//
    void visit(ast::Assign &node) override;//
    void visit(ast::Formal &node) override;//
    void visit(ast::Formals &node) override;//
    void visit(ast::FuncDecl &node) override;//
    void visit(ast::Funcs &node) override;

};

#endif