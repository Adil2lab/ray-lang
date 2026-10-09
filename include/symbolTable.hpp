#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include "utils.hpp"

class SymbolTable
{
private:
    std::vector<std::unordered_map<std::string, Symbol>> scopes;

public:
    SymbolTable();

    void enterScope();

    void exitScope();

    bool declare(const std::string &name, const Symbol &symbol);

    Symbol *lookup(const std::string &name);
};
#endif // SYMBOL_TABLE_HPP
