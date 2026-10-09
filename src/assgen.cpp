#include "assgen.hpp"

#include <sstream>
#include <string>
#include <variant>
#include <vector>

AssGen::AssGen(const Platform &_platform, const std::vector<std::string> &_libraries) : platform(_platform), libraries(_libraries) {}

// -- Windows 64 -- start --

void AssGen::gen_drectiveHeader(const std::vector<std::string> &_libraries, std::stringstream &out)
{
    // ==============================================================
    //  COFF DIRECTIVE SECTION(Autolink metadata read by lld - link)
    // ==============================================================
    out << "section .drective info\n";
    out << "    db \'";
    for (std::string lib : _libraries)
    {
        out << "/DEFAULTLIB:" << lib << ' ';
    }
    out << "\'\n";
    return;
}

void AssGen::gen_win_RetStmt(NodeRet &node, std::stringstream &out)
{
    out << "    .global main\nmain:\n";
    out << "    mov rax, 60\n";
    Gen_ExpVisitor exp_visitor{out};
    std::visit(exp_visitor, node.exp.token);
    out << "    syscall\n";
    return;
}

// -- Windows 64 -- end --
// -- Linux 64 -- start --

void AssGen::gen_lin_RetStmt(NodeRet &node, std::stringstream &out)
{
    out << "    .global main\nmain:\n";
    out << "    mov rax, 60\n";
    Gen_ExpVisitor exp_visitor{out};
    std::visit(exp_visitor, node.exp.token);
    out << "    syscall\n";
    return;
}
void AssGen::gen_lin_VarDeclStmt(NodeVarDecl &node, std::stringstream &out)
{
    int quantity;
    std::string byteSize;
    if (node.dataType == DataType::Int)
    {
        quantity = 4;
        byteSize = "dword";
    }
    else if (node.dataType == DataType::Char)
    {
        quantity = 1;
        byteSize = "byte";
    }
    out << "    sub rsp," << quantity << "\n";
    out << "    mov " << byteSize << "[rsp]," << node.exp.value().token.value.value() << "\n";

    return;
}

// -- Linux 64 -- end --

int AssGen::gen_Progam(std::stringstream &assemblyCode)
{
}