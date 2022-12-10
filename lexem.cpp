#include "lexem.h"

Lexem::Lexem(LexemType lexemType_, OperationType operationType_, RelationType relationType_)
    : m_lexemType(lexemType_), m_opType(operationType_), m_relationType(relationType_)
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


