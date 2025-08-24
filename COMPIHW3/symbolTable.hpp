#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include<vector>
#include<string>
#include<iostream>
#include <cstring>
#include "symbol.hpp"

using namespace std;

class SymbolTable {

public:
    vector<symbol> symbols; 
    int currentScope; 
    int nextPosition; 

    SymbolTable() : currentScope(0), nextPosition(0) {}

    
    SymbolTable(const SymbolTable& other)
        : symbols(other.symbols),
          currentScope(other.currentScope),
          nextPosition(other.nextPosition) {}

    
    void addSymbol(const string &name, const string &type, bool isFunction = false, bool isDefined = false) {
        symbols.emplace_back(name, type, isFunction, isDefined, nextPosition++);
    }
    // Add a symbol by reference
    void addSymbol(const symbol& s) {
        symbols.push_back(s);
    }
    symbol* findSymbol(const string &name) {
        for (int i = 0; i < symbols.size(); ++i) {
            symbol& sym = symbols[i];
            if (sym.name == name) {
                return &sym;
            }
        }
        return nullptr; 
    }
    string getSymbolTypeByName(const string &name) const {
        for (int i = 0; i < symbols.size(); ++i) {
            const symbol& sym = symbols[i];
            if (sym.name == name) {
                return sym.type;
            }
        }
        return ""; 
    }
    bool isSymbolFunction(const string &name) const {
        for (int i = 0; i < symbols.size(); ++i) {
            const symbol& sym = symbols[i];
            if (sym.name == name) {
                return sym.isFunction;
            }
        }
        return false; 
    }

}; // <-- semicolon here
#endif



