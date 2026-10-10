#include "tokenizer.hpp"

#include <cctype>
#include <cstdlib>
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
        m_col = 1;
    }
    else
    {
        m_col++;
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
            size_t line = m_line;
            size_t col = m_col;
            buff.push_back(consume());
            while (peek().has_value() && std::isalnum(peek().value()))
            {
                buff.push_back(consume());
            }
            if (buff == "return")
            {
                tokens.push_back({TokenKind::_return, std::nullopt, line, col});
                buff.clear();
                continue;
            }
            else if (buff == "fn")
            {
                tokens.push_back({TokenKind::_function, buff, line, col});
                buff.clear();
                continue;
            }
            else if (buff == "class")
            {
                tokens.push_back({TokenKind::_class, buff, line, col});
                buff.clear();
                continue;
            }
            else if (buff == "int" || buff == "float" || buff == "string" || buff == "char")
            {
                DataType dt;
                switch (buff[0])
                {
                case 'i':
                    dt = DataType::Int;
                    break;
                case 'f':
                    dt = DataType::Float;
                    break;
                case 's':
                    dt = DataType::String;
                    break;
                case 'c':
                    dt = DataType::Char;
                    break;
                default:
                    std::cerr << "Unknown data type: " << buff << std::endl;
                    exit(EXIT_FAILURE);
                }
                tokens.push_back({TokenKind::DataType, dt, line, col});
                buff.clear();
                continue;
            }
            else
            {
                tokens.push_back({TokenKind::Identifier, buff, line, col});
                buff.clear();
                continue;
            }
        }
        else if (std::isdigit(peek().value()))
        {
            size_t line = m_line;
            size_t col = m_col;
            buff.push_back(consume());
            while (peek().has_value() && std::isdigit(peek().value()))
            {
                buff.push_back(consume());
            }
            tokens.push_back({TokenKind::Int_lit, buff, line, col});
            buff.clear();
            continue;
        }
        else if (peek().value() == '=')
        {
            size_t line = m_line;
            size_t col = m_col;
            consume();
            tokens.push_back({TokenKind::Symbol, std::string("="), line, col});
            continue;
        }
        else if (peek().value() == '"')
        {
            size_t line = m_line;
            size_t col = m_col;
            consume();
            while (peek().has_value() && peek().value() != '"')
            {
                buff.push_back(consume());
            }
            consume();
            tokens.push_back({TokenKind::String_lit, buff, line, col});
            buff.clear();
            continue;
        }
        else if (peek().value() == ';')
        {
            size_t line = m_line;
            size_t col = m_col;
            consume();
            tokens.push_back({TokenKind::SemCln, std::nullopt, line, col});
            continue;
        }
        else if (peek().value() == '(')
        {
            size_t line = m_line;
            size_t col = m_col;
            consume();
            tokens.push_back({TokenKind::OpenParen, std::nullopt, line, col});
            continue;
        }
        else if (peek().value() == ')')
        {
            size_t line = m_line;
            size_t col = m_col;
            consume();
            tokens.push_back({TokenKind::CloseParen, std::nullopt, line, col});
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