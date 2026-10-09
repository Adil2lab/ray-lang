//
// Created by adil on 9/6/25.
//

#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

#include "utils.hpp"

class Parser
{
public:
    explicit Parser(std::vector<Token> tokens);

    // Token parse_paren(const int& line) {

    // }

    std::optional<NodeRetExp> parse_retExp();

    std::optional<NodeRet> parse_ret();

    std::optional<ExpVarDecl> parse_expVarDecl();

    std::vector<NodeVarDecl> parse_varDecl();

private:
    [[nodiscard]] std::optional<Token> peak(int offset = 0) const;

    Token consume(bool eat = false);

    std::vector<Token> tokens;
    size_t m_index = 0;
};
#endif // PARSER_HPP
