#ifndef RGR2_VIRTUAL_MACHINE_H
#define RGR2_VIRTUAL_MACHINE_H
#include <vector>
#include <unordered_set>
#include <map>
#include <stack>
#include <string>
#include "lexer.h"

using byte = unsigned char;
class VirtualMachine
{
private:
    enum class VarType
    {
        Int, MyType
    };
    enum class Code : byte
    {
        Empty = 0, Push, Pop, Jmp, Ji, Read, Write, Plus, Minus, Multiply, Div, Mod, Equal, NotEqual, Less, Greater,
        LessEqual, GreaterEqual, End,
    };
    struct Var
    {
        VarType type;
        void* data;
    };
public:
    explicit VirtualMachine(std::vector<Lexem> lexems_);
    std::vector<byte> GenerateByteCode();
    void Run(const std::vector<byte>& bytecode_);
private:
    static byte getLexemSizeInBytecode(const Lexem& lexem_);
    size_t getOffsetFromBegin(size_t str);
    Code getLexemCode(const Lexem& lexem_);
    void div();
    void mod();
    void multiply();
    void minus();
    void plus();
    void equal();
    void notEqual();
    void greater();
    void less();
    void lessEqual();
    void greaterEqual();
private:
    std::stack<size_t> m_stack;
    size_t m_currentPosStack;
    std::unordered_set<size_t> m_constants;
    std::map<std::string, size_t> m_tableVars;
    std::vector<Lexem> m_lexems;
};


#endif //RGR2_VIRTUAL_MACHINE_H
