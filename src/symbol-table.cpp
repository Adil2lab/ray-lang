#include "symbolTable.hpp"

#include <unordered_map>
#include <vector>

SymbolTable::SymbolTable()
{
    // Start with a global scope
    scopes.emplace_back();
}

void SymbolTable::enterScope()
{
    scopes.emplace_back();
}

void SymbolTable::exitScope()
{
    if (!scopes.empty())
    {
        scopes.pop_back();
    }
}

bool SymbolTable::declare(const std::string &name, const Symbol &symbol)
{
    if (scopes.empty())
        return false; // No scope to declare in
    auto &currentScope = scopes.back();
    if (currentScope.find(name) != currentScope.end())
    {
        return false; // Symbol already declared in the current scope
    }
    currentScope[name] = symbol;
    return true;
}

Symbol *SymbolTable::lookup(const std::string &name)
{
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
    {
        auto found = it->find(name);
        if (found != it->end())
        {
            return &found->second;
        }
    }
    return nullptr; // Not found in any scope
}
