//
// Created by adil on 9/6/25.
//

#ifndef UTILS_RAYL_HPP
#define UTILS_RAYL_HPP

#include <cstddef>
#include <optional>
#include <variant>
#include <string>
#include <vector>

enum class DataType {
    Int,
    Float,
    String,
    Char
};

// -- Lexer Parts --

enum class TokenKind {
    DataType,
    Identifier,
    Int_lit,
    String_lit,
    Symbol,
    SemCln,
    _class,
    _function,
    _return,
    OpenParen,
    CloseParen,
    EndofFile,
    _deleted,
    _moved
};

struct Token {
    TokenKind type;
    std::optional<std::variant<std::string, DataType>> value;
    size_t line;
    size_t column;
};

// -- End --

// -- Parser Parts --

// Expressions

// Represents a variable declaration expression in the AST.
struct ExpVarDecl {
    Token token;
};

/**
 * Represents an expression for return statement that doesn't have identifier.
 */
struct ExpRetNIdent {
    Token token;
};

/**
 * Represents an expression for return statement that is defined by a identifier.
 */
struct ExpRetIdent {
    Token token;
};

// Represents a return expression node in the AST.
struct ExpNodeRet {
    std::variant<ExpRetNIdent, ExpRetIdent> token;
};

// Nodes

/**
 * Represents a variable declaration node in the AST.
 */
struct NodeVarDecl {
    DataType dataType;
    std::string identifier;
    std::optional<ExpVarDecl> exp;
};

/**
 * Represents a return node in the AST.
 */
struct NodeRet {
    ExpNodeRet exp;
};

struct Stmt {
    std::variant<NodeVarDecl, NodeRet> stmt;
};

struct NodeProgram {
    std::vector<Stmt> stmts;
};

// -- End --

enum class Platform {
    Windows64,
    Linux64,
    MacOS,
    NotSure
};

enum class SymbolType {
    Variable,
    Function,
    Parameter
};

enum class StorageType {
    Global,
    Static,
    Stack
};

struct Symbol {
    std::string type;
    SymbolType symbolType;
    StorageType storageType;
    int offset;
    size_t size;
    bool isMutable;
};

#endif // UTILS_RAYL_HPP
