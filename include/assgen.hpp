//
// Created by adil on 9/7/25.
//

#ifndef ASS_GEN_RAYL_HPP
#define ASS_GEN_RAYL_HPP

#include "utils.hpp"
#include "expVisitor.hpp"

class AssGen
{
public:
    explicit AssGen(const Platform &_platform, const std::vector<std::string> &_libraries);

    Platform platform;
    std::vector<std::string> libraries;

    // -- Windows 64 -- start --

    void gen_drectiveHeader(const std::vector<std::string> &_libraries, std::stringstream &out);

    [[nodiscard]] void gen_win_RetStmt(NodeRet &node, std::stringstream &out);

    // -- Windows 64 -- end --
    // -- Linux 64 -- start --

    [[nodiscard]] void gen_lin_RetStmt(NodeRet &node, std::stringstream &out);
    [[nodiscard]] void gen_lin_VarDeclStmt(NodeVarDecl &node, std::stringstream &out);
    // -- Linux 64 -- end --

    int gen_Progam(std::stringstream &assemblyCode);

private:
};
#endif // ASS_GEN_RAYL_HPP
