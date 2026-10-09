//
// Created by adil on 9/6/25.
//

#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include "utils.hpp"

class Tokenizer
{
public:
    explicit Tokenizer(std::string &src);
    std::vector<Token> tokenize();

private:
    [[nodiscard]] std::optional<char> peek(int offset = 0) const;

    char consume();

    const std::string m_src;
    size_t m_index = 0;
    size_t m_line = 1;
    size_t m_token = 1;
};
#endif // TOKENIZER_HPP
