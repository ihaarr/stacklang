#include <iostream>
#include "lexem.h"

Lexem::Lexem(LexemType lexemType_, size_t strNum, const std::string& varName_, VarType varType, int constValue_, OperationType operationType_, RelationType relationType_)
    : m_lexemType(lexemType_), m_stringNumber(strNum), m_varName(varName_), m_constValue(constValue_),
    m_opType(operationType_), m_relationType(relationType_)
{

}
LexemType Lexem::GetLexemType() const
{
    return m_lexemType;
}
OperationType Lexem::GetOperationType() const
{
    return m_opType;
}
RelationType Lexem::GetRelationType() const
{
    return m_relationType;
}
std::string Lexem::getOperationInfo() const
{
    switch(m_opType)
    {
        case OperationType::Plus: return std::string("+");
        case OperationType::Minus: return std::string("-");
        case OperationType::Div: return std::string("/");
        case OperationType::Mod: return std::string("%");
        case OperationType::Multiply: return std::string("*");
        default: return std::string("undefined");
    }
}
std::string Lexem::getRelationInfo() const
{
    switch(m_relationType)
    {
        case RelationType::Equal: return std::string("=");
        case RelationType::NotEqual: return std::string("!=");
        case RelationType::Less: return std::string("<");
        case RelationType::Greater: return std::string(">");
        case RelationType::LessEqual: return std::string("<=");
        case RelationType::GreaterEqual: return std::string(">=");
        default: return std::string("");
    }
}
std::string Lexem::ToString() const
{
    switch(m_lexemType)
    {
        case LexemType::Constant: return std::string("Lexem: constant; value = ") + std::to_string(m_constValue);
        case LexemType::Var: return std::string("Lexem: var; name = ") + m_varName;
        case LexemType::Push: return std::string("Lexem: push");
        case LexemType::Pop: return std::string("Lexem: pop");
        case LexemType::Read: return std::string("Lexem: read");
        case LexemType::Write: return std::string("Lexem: write");
        case LexemType::Op: return std::string("Lexem: op; value = ") + getOperationInfo();
        case LexemType::Relation: return std::string("Lexem: relation; value = ") + getRelationInfo();
        case LexemType::Jmp: return std::string("Lexem: jmp");
        case LexemType::Ji: return std::string("Lexem: ji");
        case LexemType::End: return std::string("Lexem: end");
        case LexemType::Pol: return std::string("Lexem: pol");
        case LexemType::EndOfFile: return std::string("Lexem: eof");
        default: return std::string("Lexem: error");
    }
}

std::ostream& operator<<(std::ostream& os_, const Lexem& lexem_)
{
    os_ << lexem_.ToString();
    return os_;
}
std::string Lexem::GetVarName() const
{
    return m_varName;
}
size_t Lexem::GetStringNumber() const
{
    return m_stringNumber;
}
size_t Lexem::GetConstValue() const
{
    return m_constValue;
}
VarType Lexem::GetVarType() const
{
    return m_varType;
}


