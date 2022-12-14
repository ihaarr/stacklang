#include <utility>
#include "virtual_machine.h"

VirtualMachine::VirtualMachine()
{
}
size_t VirtualMachine::getOffsetFromBegin(const std::vector<Lexem>& lexems_, size_t str)
{
    size_t offset = 0;
    for(size_t i = 0; i < lexems_.size(); ++i)
    {
        if(lexems_[i].GetStringNumber() >= str)
            break;
        offset += getLexemSizeInBytecode(lexems_[i]);
    }
    return offset;
}
std::pair<VirtualMachine::Var, VirtualMachine::Var> VirtualMachine::getTwoVarsFromStack()
{
    Var b = m_stack.top();
    m_stack.pop();
    Var a = m_stack.top();
    m_stack.pop();
    return {std::move(a), std::move(b)};
}
void VirtualMachine::plus()
{
    auto pair = getTwoVarsFromStack();
    if(pair.first.type != pair.second.type)
        throw 100;

    switch(pair.first.type)
    {
        case VarType::Int:
        {
            m_stack.push(Var(VarType::Int, pair.first.dataInt + pair.second.dataInt, {}));
            break;
        }
        case VarType::MyType:
        {
            m_stack.push(Var(VarType::MyType, 0, pair.first.dataPol + pair.second.dataPol));
            break;
        }
        default:
            throw 10;
    }
}
void VirtualMachine::minus()
{
    auto pair = getTwoVarsFromStack();
    if(pair.first.type != pair.second.type)
        throw 100;

    switch(pair.first.type)
    {
        case VarType::Int:
        {
            m_stack.push(Var(VarType::Int, pair.first.dataInt - pair.second.dataInt, {}));
            break;
        }
        case VarType::MyType:
        {
            m_stack.push(Var(VarType::MyType, 0, pair.first.dataPol - pair.second.dataPol));
            break;
        }
        default:
            throw 10;
    }
}
void VirtualMachine::multiply()
{
    auto pair = getTwoVarsFromStack();
    if(pair.first.type != pair.second.type)
        throw 100;

    switch(pair.first.type)
    {
        case VarType::Int:
        {
            m_stack.push(Var(VarType::Int, pair.first.dataInt * pair.second.dataInt, {}));
            break;
        }
        case VarType::MyType:
        {
            m_stack.push(Var(VarType::MyType, 0, pair.first.dataPol * pair.second.dataPol));
            break;
        }
        default:
            throw 10;
    }
}
void VirtualMachine::mod()
{
    auto pair = getTwoVarsFromStack();
    if(pair.first.type != pair.second.type)
        throw 100;

    switch(pair.first.type)
    {
        case VarType::Int:
        {
            m_stack.push(Var(VarType::Int, pair.first.dataInt % pair.second.dataInt, {}));
            break;
        }
        case VarType::MyType:
        {
            m_stack.push(Var(VarType::MyType, 0, pair.first.dataPol % pair.second.dataPol));
            break;
        }
        default:
            throw 10;
    }
}
void VirtualMachine::div()
{
    auto pair = getTwoVarsFromStack();
    if(pair.first.type != pair.second.type)
        throw 100;

    switch(pair.first.type)
    {
        case VarType::Int:
        {
            m_stack.push(Var(VarType::Int, pair.first.dataInt / pair.second.dataInt, {}));
            break;
        }
        case VarType::MyType:
        {
            m_stack.push(Var(VarType::MyType, 0, pair.first.dataPol / pair.second.dataPol));
            break;
        }
        default:
            throw 10;
    }
}
void VirtualMachine::equal()
{
    auto pair = getTwoVarsFromStack();
    if(pair.first.type != pair.second.type)
        throw 100;

    switch(pair.first.type)
    {
        case VarType::Int:
        {
            m_stack.push(Var(VarType::Int, pair.first.dataInt == pair.second.dataInt, {}));
            break;
        }
        case VarType::MyType:
        {
            m_stack.push(Var(VarType::Int, pair.first.dataPol == pair.second.dataPol, {}));
            break;
        }
        default:
            throw 10;
    }
}
void VirtualMachine::notEqual()
{
    auto pair = getTwoVarsFromStack();
    if(pair.first.type != pair.second.type)
        throw 100;

    switch(pair.first.type)
    {
        case VarType::Int:
        {
            m_stack.push(Var(VarType::Int, pair.first.dataInt != pair.second.dataInt, {}));
            break;
        }
        case VarType::MyType:
        {
            m_stack.push(Var(VarType::Int, pair.first.dataPol != pair.second.dataPol, {}));
            break;
        }
        default:
            throw 10;
    }
}
void VirtualMachine::greater()
{
    auto pair = getTwoVarsFromStack();
    if(pair.first.type != pair.second.type)
        throw 100;

    switch(pair.first.type)
    {
        case VarType::Int:
        {
            m_stack.push(Var(VarType::Int, pair.first.dataInt > pair.second.dataInt, {}));
            break;
        }
        default:
            throw 105;
    }
}
void VirtualMachine::less()
{
    auto pair = getTwoVarsFromStack();
    if(pair.first.type != pair.second.type)
        throw 100;

    switch(pair.first.type)
    {
        case VarType::Int:
        {
            m_stack.push(Var(VarType::Int, pair.first.dataInt > pair.second.dataInt, {}));
            break;
        }
        default:
            throw 105;
    }
}
void VirtualMachine::lessEqual()
{
    auto pair = getTwoVarsFromStack();
    if(pair.first.type != pair.second.type)
        throw 100;

    switch(pair.first.type)
    {
        case VarType::Int:
        {
            m_stack.push(Var(VarType::Int, pair.first.dataInt <= pair.second.dataInt, {}));
            break;
        }
        default:
            throw 105;
    }
}
void VirtualMachine::greaterEqual()
{
    auto pair = getTwoVarsFromStack();
    if(pair.first.type != pair.second.type)
        throw 100;

    switch(pair.first.type)
    {
        case VarType::Int:
        {
            m_stack.push(Var(VarType::Int, pair.first.dataInt >= pair.second.dataInt, {}));
            break;
        }
        default:
            throw 105;
    }
}
void VirtualMachine::generateByteCode(const std::vector<Lexem>& lexems_)
{
    size_t size = 0;
    for(const auto& lexem : lexems_)
    {
        size += getLexemSizeInBytecode(lexem);
    }
    m_bytecode.reserve(size);
    for(size_t i = 0; i < lexems_.size(); ++i)
    {
        Code code = getLexemCode(lexems_[i]);
        switch(code)
        {
            case Code::Push:
            case Code::Pop:
            {
                ++i;
                LexemType lexemType = lexems_[i].GetLexemType();
                if(lexemType == LexemType::Constant)
                {
                    m_bytecode.push_back(static_cast<byte>(code));
                    m_constants.emplace_back(VarType::Int, lexems_[i].GetConstValue(), Polynominal());
                    auto* varPtr = &m_constants.back();
                    byte* val = reinterpret_cast<byte*>(&varPtr);
                    for(size_t k = 0; k < sizeof(int*); ++k)
                    {
                        m_bytecode.push_back(val[k]);
                    }
                }
                else if(lexemType == LexemType::Var)
                {
                    m_bytecode.push_back(static_cast<byte>(code));
                    auto varPtr = &m_tableVars.insert({lexems_[i].GetVarName(), Var(lexems_[i].GetVarType(), lexems_[i].GetConstValue(), {})}).first->second;
                    byte* var = reinterpret_cast<byte*>(&varPtr);
                    for(size_t k = 0; k < sizeof(Var*); ++k)
                    {
                        m_bytecode.push_back(var[k]);
                    }
                }
                break;
            }
            case Code::Jmp:
            case Code::Ji:
            {
                ++i;
                LexemType lexemType = lexems_[i].GetLexemType();
                if(lexemType == LexemType::Constant)
                {
                    m_bytecode.push_back(static_cast<char>(code));
                    size_t numStr = lexems_[i].GetConstValue();
                    for(size_t m = 0; i < lexems_.size(); ++m)
                    {
                        size_t lexemStr = lexems_[m].GetStringNumber();
                        if(lexemStr >= numStr)
                        {
                            for(int k = 64; k > 0; k -= 8)
                            {
                                m_bytecode.push_back(numStr >> k);
                            }
                            break;
                        }
                    }
                }
                break;
            }
            default:
                m_bytecode.push_back(static_cast<byte>(code));
        }
    }
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
        case LexemType::Pol: return Code::Pol;
        default: return Code::Empty;
    }
}
void VirtualMachine::Run(const std::vector<Lexem>& lexems_)
{
    generateByteCode(lexems_);
    auto begin = const_cast<byte*>(m_bytecode.data());
    for(size_t i = 0; i < m_bytecode.size(); ++i)
    {
        Code code = static_cast<Code>(m_bytecode[i]);
        switch(code)
        {
            case Code::Push:
            {
                Var* var = *reinterpret_cast<Var**>(begin + i + 1);
                i += sizeof(Var*);
                m_stack.push(*var);
                break;
            }
            case Code::Pop:
            {
                Var* var = *reinterpret_cast<Var**>(begin + i + 1);
                i += sizeof(int*);
                *var = m_stack.top();
                m_stack.pop();
                break;
            }
            case Code::Write:
            {
                std::cout << m_stack.top();
                m_stack.pop();
                break;
            }
            case Code::Pol:
            {
                Var count = m_stack.top();
                m_stack.pop();
                if(count.type == VarType::MyType)
                    throw 201;

                Polynominal pol;
                for(size_t i = 0; i < count.dataInt; ++i)
                {
                    Var degree = m_stack.top();
                    m_stack.pop();
                    Var coeff = m_stack.top();
                    m_stack.pop();
                    if(degree.type != VarType::Int || coeff.type != VarType::Int)
                        throw 500;

                    pol.Push(degree.dataInt, coeff.dataInt);
                }
                m_stack.push(Var(VarType::MyType, 0, pol));
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
                i = getOffsetFromBegin(lexems_, str) - 1;
                break;
            }
            case Code::Ji:
            {
                size_t str = *reinterpret_cast<size_t*>(begin + i + 1);
                Var& top = m_stack.top();
                if(top.type != VarType::Int)
                    throw 100;
                if(top.dataInt > 0)
                {
                    i = getOffsetFromBegin(lexems_, str) - 1;
                }
                else
                {
                    i += sizeof(size_t*);
                }
                m_stack.pop();
                break;
            }
            case Code::Read:
            {
                int val;
                std::cin >> val;
                m_stack.push(Var(VarType::Int, val, {}));
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
                i = m_bytecode.size();
                break;
            }
        }
    }
}
