#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include<string>
#include<cstring>


using namespace std;

class symbol
{
public:
    std::string name;
    std::string type;
    std::string retValueType; // type of the return value for functions; 
    bool isFunction; // true if it's a function, false if it's a variable
    bool isDefined; // true if it's defined, false otherwise
    int position; // position in the symbol table

  symbol(string name, string type, bool isFunction = false, bool isDefined = false, int position = -1)
      : name(name), type(type), isFunction(isFunction), isDefined(isDefined), position(position) {}

    // Default constructor
    symbol() = default;

    // Destructor
    ~symbol() = default;
};

#endif
