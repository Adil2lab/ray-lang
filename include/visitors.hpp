#ifndef VISITORS_RAYL_HPP
#define VISITORS_RAYL_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <variant>

#include "utils.hpp"

struct Gen_ExpVisitor {
    std::stringstream& out;

    void operator()(const ExpRetNIdent& exp) {
        std::string value = std::get<std::string>(exp.token.value.value());
        out << "    mov rdi, " << value << "\n";
    }
    void operator()(const ExpRetIdent& exp) {
        std::string value = std::get<std::string>(exp.token.value.value());
        out << "    mov rdi, " << value << "\n";
    }
};

struct Tok_ValVisitor
{
    std::string operator()(const std::string& str) const
    {
        return str;
    }
    DataType operator()(const DataType& dt) const
    {
        return dt;
    }
};

#endif // VISITORS_RAYL_HPP
