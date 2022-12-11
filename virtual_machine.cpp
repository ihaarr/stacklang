#include <utility>
#include "virtual_machine.h"

VirtualMachine::VirtualMachine(std::vector<Lexem> lexems_) : m_lexems(std::move(lexems_))
{

}
size_t VirtualMachine::getOffsetFromBegin(size_t str)
{
    size_t offset = 0;
    for(size_t i = 0; i < m_lexems.size(); ++i)
    {
        if(m_lexems[i].GetStringNumber() >= str)
            break;
        offset += getLexemSizeInBytecode(m_lexems[i]);
    }
    return offset;
}
void VirtualMachine::plus()
{
    int b = m_stack.top();
    m_stack.pop();
    int a = m_stack.top();
    m_stack.pop();
    m_stack.push(a + b);
}
void VirtualMachine::minus()
{
    int b = m_stack.top();
    m_stack.pop();
    int a = m_stack.top();
    m_stack.pop();
    m_stack.push(a - b);
}
void VirtualMachine::multiply()
{
    int b = m_stack.top();
    m_stack.pop();
    int a = m_stack.top();
    m_stack.pop();
    m_stack.push(a * b);
}
void VirtualMachine::mod()
{
    int b = m_stack.top();
    m_stack.pop();
    int a = m_stack.top();
    m_stack.pop();
    m_stack.push(a % b);
}
void VirtualMachine::div()
{
    int b = m_stack.top();
    m_stack.pop();
    int a = m_stack.top();
    m_stack.pop();
    m_stack.push(a / b);
}
void VirtualMachine::equal()
{
    int b = m_stack.top();
    m_stack.pop();
    int a = m_stack.top();
    m_stack.pop();
    m_stack.push(a == b);
}
void VirtualMachine::notEqual()
{
    int b = m_stack.top();
    m_stack.pop();
    int a = m_stack.top();
    m_stack.pop();
    m_stack.push(a != b);
}
void VirtualMachine::greater()
{
    int b = m_stack.top();
    m_stack.pop();
    int a = m_stack.top();
    m_stack.pop();
    m_stack.push(a > b);
}
void VirtualMachine::less()
{
    int b = m_stack.top();
    m_stack.pop();
    int a = m_stack.top();
    m_stack.pop();
    m_stack.push(a < b);
}
void VirtualMachine::lessEqual()
{
    int b = m_stack.top();
    m_stack.pop();
    int a = m_stack.top();
    m_stack.pop();
    m_stack.push(a <= b);
}
void VirtualMachine::greaterEqual()
{
    int b = m_stack.top();
    m_stack.pop();
    int a = m_stack.top();
    m_stack.pop();
    m_stack.push(a >= b);
}
std::vector<byte> VirtualMachine::GenerateByteCode()
{
    std::vector<byte> bytecode;
    size_t size = 0;
    for(const auto& lexem : m_lexems)
    {
        size += getLexemSizeInBytecode(lexem);
    }
    bytecode.reserve(size);
    for(size_t i = 0; i < m_lexems.size(); ++i)
    {
        Code code = getLexemCode(m_lexems[i]);
        switch(code)
        {
            case Code::Push:
            case Code::Pop:
            {
                ++i;
                LexemType lexemType = m_lexems[i].GetLexemType();
                if(lexemType == LexemType::Constant)
                {
                    bytecode.push_back(static_cast<byte>(code));
                    const size_t* b = &*m_constants.insert(m_lexems[i].GetConstValue()).first;
                    byte* val = reinterpret_cast<byte*>(&b);
                    for(size_t k = 0; k < sizeof(int*); ++k)
                    {
                        bytecode.push_back(val[k]);
                    }
                }
                else if(lexemType == LexemType::Var)
                {
                    bytecode.push_back(static_cast<byte>(code));
                    size_t* varPtr = &m_tableVars[m_lexems[i].GetVarName()];
                    byte* var = reinterpret_cast<byte*>(&varPtr);
                    for(size_t k = 0; k < sizeof(Var*); ++k)
                    {
                        bytecode.push_back(var[k]);
                    }
                }
                break;
            }
            case Code::Jmp:
            case Code::Ji:
            {
                ++i;
                LexemType lexemType = m_lexems[i].GetLexemType();
                if(lexemType == LexemType::Constant)
                {
                    bytecode.push_back(static_cast<char>(code));
                    size_t numStr = m_lexems[i].GetConstValue();
                    for(size_t m = 0; i < m_lexems.size(); ++m)
                    {
                        size_t lexemStr = m_lexems[m].GetStringNumber();
                        if(lexemStr >= numStr)
                        {
                            for(int k = 64; k > 0; k -= 8)
                            {
                                bytecode.push_back(numStr >> k);
                            }
                            break;
                        }
                    }
                }
                break;
            }
            default:
                bytecode.push_back(static_cast<byte>(code));
        }
    }
    return bytecode;
}
byte VirtualMachine::getLexemSizeInBytecode(const Lexem& lexem_)
{
    switch (lexem_.GetLexemType())
    {
        case LexemType::Constant:
        case LexemType::Var: return 8;
        default: return 1;
    }
}
VirtualMachine::Code VirtualMachine::getLexemCode(const Lexem &lexem_)
{
    switch(lexem_.GetLexemType())
    {
        case LexemType::Push: return Code::Push;
        case LexemType::Pop: return Code::Pop;
        case LexemType::Op:
        {
            switch(lexem_.GetOperationType())
            {
                case OperationType::Undefined:
                case OperationType::Plus: return Code::Plus;
                case OperationType::Minus: return Code::Minus;
                case OperationType::Multiply: return Code::Multiply;
                case OperationType::Div: return Code::Div;
                case OperationType::Mod: return Code::Mod;
            }
        }
        case LexemType::Relation:
        {
            switch(lexem_.GetRelationType())
            {
                case RelationType::Undefined:
                case RelationType::Equal: return Code::Equal;
                case RelationType::NotEqual: return Code::NotEqual;
                case RelationType::Less: return Code::Less;
                case RelationType::Greater: return Code::Greater;
                case RelationType::LessEqual: return Code::LessEqual;
                case RelationType::GreaterEqual: return Code::GreaterEqual;
            }
        }
        case LexemType::Jmp: return Code::Jmp;
        case LexemType::Ji: return Code::Ji;
        case LexemType::Read: return Code::Read;
        case LexemType::Write: return Code::Write;
        case LexemType::End: return Code::End;
        case LexemType::EndOfFile: return Code::End;
        default: return Code::Empty;
    }
}
void VirtualMachine::Run(const std::vector<byte>& bytecode_)
{
    auto begin = const_cast<byte*>(bytecode_.data());
    for(size_t i = 0; i < bytecode_.size(); ++i)
    {
        Code code = static_cast<Code>(bytecode_[i]);
        switch(code)
        {
            case Code::Push:
            {
                size_t* var = *reinterpret_cast<size_t**>(begin + i + 1);
                i += sizeof(size_t*);
                m_stack.push(*var);
                break;
            }
            case Code::Pop:
            {
                size_t* var = *reinterpret_cast<size_t**>(begin + i + 1);
                i += sizeof(int*);
                *var = m_stack.top();
                m_stack.pop();
                break;
            }
            case Code::Write:
            {
                std::cout << m_stack.top() << std::endl;
                m_stack.pop();
                break;
            }
            case Code::Plus:
            {
                plus();
                break;
            }
            case Code::Minus:
            {
                minus();
                break;
            }
            case Code::Multiply:
            {
                multiply();
                break;
            }
            case Code::Div:
            {
                div();
                break;
            }
            case Code::Mod:
            {
                mod();
                break;
            }
            case Code::Jmp:
            {
                size_t str = *reinterpret_cast<size_t*>(begin + i + 1);
                i = getOffsetFromBegin(str) - 1;
                break;
            }
            case Code::Ji:
            {
                if(m_stack.top() > 0)
                {
                    size_t str = *reinterpret_cast<size_t*>(begin + i + 1);
                    i = getOffsetFromBegin(str) - 1;
                }
                else
                {
                    i += sizeof(int*);
                }
                m_stack.pop();
                break;
            }
            case Code::Read:
            {
                size_t val;
                std::cin >> val;
                m_stack.push(val);
                break;
            }
            case Code::Equal:
            {
                equal();
                break;
            }
            case Code::NotEqual:
            {
                notEqual();
                break;
            }
            case Code::Less:
            {
                less();
                break;
            }
            case Code::Greater:
            {
                greater();
                break;
            }
            case Code::LessEqual:
            {
                lessEqual();
                break;
            }
            case Code::GreaterEqual:
            {
                greaterEqual();
                break;
            }
            case Code::End:
            {
                i = bytecode_.size();
                break;
            }
        }
    }
}

