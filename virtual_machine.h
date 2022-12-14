#ifndef RGR2_VIRTUAL_MACHINE_H
#define RGR2_VIRTUAL_MACHINE_H
#include <vector>
#include <list>
#include <map>
#include <stack>
#include <string>
#include "lexer.h"
#include "polynominal.h"

using byte = unsigned char;
class VirtualMachine
{
private:
    enum class Code : byte
    {
        Empty = 0, Push, Pop, Jmp, Ji, Read, Write, Plus, Minus, Multiply, Div, Mod, Equal, NotEqual, Less, Greater,
        LessEqual, GreaterEqual, Pol, End, ValueFunc, DerivativeFunc, DegreeFunc
    };
    struct Var
    {
        Var(VarType type_, ssize_t dataInt_, Polynominal dataPol_) : type(type_), dataInt(dataInt_), dataPol(dataPol_) {}
        Var() : type(VarType::Int), dataInt(0), dataPol() {}
        VarType type;
        ssize_t dataInt;
        Polynominal dataPol;
        friend std::ostream& operator<<(std::ostream& os_, const Var& var_)
        {
            switch(var_.type)
            {
                case VarType::Int:
                {
                    std::cout << var_.dataInt << std::endl;
                    break;
                }
                case VarType::MyType:
                {
                    std::cout << var_.dataPol << std::endl;
                    break;
                }
                default:
                    break;
            }
            return os_;
        }
    };
public:
    explicit VirtualMachine();
    void Run(const std::vector<Lexem>& lexems_);
private:
    void generateByteCode(const std::vector<Lexem>& lexems_);
    std::pair<Var, Var> getTwoVarsFromStack();
    static byte getLexemSizeInBytecode(const Lexem& lexem_);
    size_t getOffsetFromBegin(const std::vector<Lexem>& lexems_, size_t str);
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
    std::stack<Var> m_stack;
    size_t m_currentPosStack;
    std::list<Var> m_constants;
    std::map<std::string, Var> m_tableVars;
    std::vector<byte> m_bytecode;
    std::vector<Lexem> m_lexems;
};


#endif //RGR2_VIRTUAL_MACHINE_H
