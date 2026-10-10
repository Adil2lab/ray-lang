#include "parser.hpp"

#include <iostream>
#include <optional>
#include <ostream>
#include <string>
#include <variant>
#include <utility>
#include <vector>
#include <cstdlib>

#include "utils.hpp"

std::optional<Token> Parser::peak(int offset) const
{
    if (m_index + offset >= tokens.size())
    {
        return {};
    }
    else
    {
        return tokens.at(m_index + offset);
    }
}

Token Parser::consume(bool eat)
{
    Token &ref = tokens.at(m_index++);
    Token a = std::move(ref);
    if (eat)
    {
        ref = Token{TokenKind::_deleted, std::nullopt, a.line, a.column};
    }
    else
    {
        ref = Token{TokenKind::_moved, std::nullopt, a.line, a.column};
    }
    return a;
}

std::optional<ExpNodeRet> Parser::parse_retExp()
{
    if (peak().has_value() && peak().value().type == TokenKind::Int_lit)
    {
        return ExpNodeRet{.token = ExpRetNIdent{consume()}};
    }
    else if (peak().has_value() && peak().value().type == TokenKind::Identifier)
    {
        return ExpNodeRet{.token = ExpRetIdent{consume()}};
    } // else if (peek().has_value() && peek().value().type == TokenType::openParen) {
    //     return NodeExp {.token = parse_paren()};
    // }
    return std::nullopt;
}

std::optional<NodeRet> Parser::parse_ret()
{
    std::optional<NodeRet> res;
    while (peak().has_value())
    {
        if (peak().value().type == TokenKind::_return)
        {
            consume();
            if (auto nodeRetExp = parse_retExp())
            {
                res = NodeRet{.exp = nodeRetExp.value()};
            }
            else
            {
                std::cerr << "Invalid expression at line " << peak().value().line << std::endl;
                exit(EXIT_FAILURE);
            }
            if (peak().has_value() && peak().value().type == TokenKind::SemCln)
            {
                consume();
                break;
            }
            else
            {
                std::cerr << "Expected ';' at line " << peak().value().line << " at column " << peak().value().column << std::endl;
                exit(EXIT_FAILURE);
            }
        }
    }
    return res;
}

std::optional<ExpVarDecl> Parser::parse_expVarDecl()
{
    if (peak().has_value() && (peak().value().type == TokenKind::Int_lit || peak().value().type == TokenKind::Identifier))
    {
        return ExpVarDecl{consume()};
    }
    return std::nullopt;
}

std::vector<NodeVarDecl> Parser::parse_varDecl()
{
    std::vector<NodeVarDecl> res;

    while (peak().has_value())
    {
        if (peak().value().type == TokenKind::DataType)
        {
            Token identifierToken;
            DataType dataTypeValue = std::get<DataType>(consume().value.value());

            if (peak().has_value() && peak().value().type == TokenKind::Identifier)
            {
                identifierToken = consume();
            }
            else
            {
                std::cerr << "Expected identifier at line " << peak().value().line << " at column " << peak().value().column << std::endl;
                exit(EXIT_FAILURE);
            }
            if (peak().has_value() && peak().value().type == TokenKind::Symbol && std::get<std::string>(peak().value().value.value()) == "=")
            {
                consume();
            }
            else
            {
                std::cerr << "Expected '=' at line " << peak().value().line << " at column " << peak().value().column << std::endl;
                exit(EXIT_FAILURE);
            }
            if (auto expVarDecl = parse_expVarDecl())
            {
                res.push_back(NodeVarDecl{
                    .dataType = dataTypeValue,
                    .identifier = std::get<std::string>(identifierToken.value.value()),
                    .exp = expVarDecl});
            }
            else
            {
                std::cerr << "Invalid expression at line " << peak().value().line << std::endl;
                exit(EXIT_FAILURE);
            }

            if (peak().has_value() && peak().value().type == TokenKind::SemCln)
            {
                consume();
            }
            else
            {
                std::cerr << "Expected ';' at line " << peak().value().line << " at column " << peak().value().column << std::endl;
                exit(EXIT_FAILURE);
            }
        }
    }
    return res;
}

Stmt Parser::parse_stmt()
{
    auto tok = peak();
    if (tok.has_value()){
        if (tok->type == TokenKind::_return) {
            auto retNode = parse_ret();
            if (retNode.has_value()) {
                return Stmt{retNode.value()};
            } else {
                std::cerr << "Failed to parse return statement at line " << tok->line << std::endl;
                exit(EXIT_FAILURE);
            }
        } else if (tok->type == TokenKind::DataType) {
            auto varDeclNodes = parse_varDecl();
            if (!varDeclNodes.empty()) {
                return Stmt{varDeclNodes.front()}; // Assuming one variable declaration per statement
            } else {
                std::cerr << "Failed to parse variable declaration at line " << tok->line << std::endl;
                exit(EXIT_FAILURE);
            }
        } else {
            std::cerr << "Unexpected token at line " << tok->line << ": " << static_cast<int>(tok->type) << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else {
        std::cerr << "Unexpected end of input while parsing statement." << std::endl;
        exit(EXIT_FAILURE);
    }
}

NodeProgram Parser::parse_program() {
    NodeProgram program;
    while (peak().has_value()) {
        program.stmts.push_back(parse_stmt());
    }
    return program;
}