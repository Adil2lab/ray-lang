#include "tokenizer.hpp"

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <optional>

Tokenizer::Tokenizer(std::string &src) : m_src(std::move(src))
{
}

std::optional<char> Tokenizer::peek(int offset) const
{
    if (m_index + offset >= m_src.size())
    {
        return {};
    }
    else
    {
        return m_src.at(m_index + offset);
    }
}

char Tokenizer::consume()
{
    char c = m_src.at(m_index++);
    if (c == '\n')
    {
        m_line++;
        m_token = 1;
    }
    else
    {
        m_token++;
    }
    return c;
}

std::vector<Token> Tokenizer::tokenize()
{
    std::string buff;
    std::vector<Token> tokens;

    while (peek().has_value())
    {
        if (std::isalpha(peek().value()))
        {
            buff.push_back(consume());
            while (peek().has_value() && std::isalnum(peek().value()))
            {
                buff.push_back(consume());
            }
            if (buff == "return")
            {
                tokens.push_back({TokenKind::_return, std::nullopt, m_line, m_token});
                buff.clear();
                continue;
            }
            else if (buff == "fn")
            {
                tokens.push_back({TokenKind::_functionNode, buff, m_line, m_token});
                buff.clear();
                continue;
            }
            else if (buff == "class")
            {
                tokens.push_back({TokenKind::_class, buff, m_line, m_token});
                buff.clear();
                continue;
            }
            else if (buff == "int" || buff == "float" || buff == "String" || buff == "char")
            {
                tokens.push_back({TokenKind::DataType, buff, m_line, m_token});
                buff.clear();
                continue;
            }
            else
            {
                tokens.push_back({TokenKind::Identifier, buff, m_line, m_token});
                buff.clear();
                continue;
            }
        }
        else if (std::isdigit(peek().value()))
        {
            buff.push_back(consume());
            while (peek().has_value() && std::isdigit(peek().value()))
            {
                buff.push_back(consume());
            }
            tokens.push_back({TokenKind::Int_lit, buff, m_line, m_token});
            buff.clear();
            continue;
        }
        else if (peek().value() == '=')
        {
            consume();
            tokens.push_back({TokenKind::Symbol, std::string("="), m_line, m_token});
            continue;
        }
        else if (peek().value() == '"')
        {
            consume();
            while (peek().has_value() && peek().value() != '"')
            {
                buff.push_back(consume());
            }
            consume();
            tokens.push_back({TokenKind::String_lit, buff, m_line, m_token});
            buff.clear();
            continue;
        }
        else if (peek().value() == ';')
        {
            consume();
            tokens.push_back({TokenKind::SemCln, std::nullopt, m_line, m_token});
            continue;
        }
        else if (peek().value() == '(')
        {
            consume();
            tokens.push_back({TokenKind::openParen, std::nullopt, m_line, m_token});
            continue;
        }
        else if (peek().value() == ')')
        {
            consume();
            tokens.push_back({TokenKind::closeParen, std::nullopt, m_line, m_token});
            continue;
        }
        else if (std::isspace(peek().value()))
        {
            consume();
            continue;
        }
        else
        {
            std::cerr << "You messed up ......" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    m_index = 0;
    return tokens;
}