#ifndef RGR2_LEXEM_H
#define RGR2_LEXEM_H

#include <cstdio>


enum class LexemType
{
    Constant, Var, Push, Pop, Op, Relation, Jmp, Ji, Read, Write, End, Comment, Error, Space, Eof,
};
enum class OperationType
{
    Undefined, Plus, Minus, Multiply, Div, Mod
};
enum class RelationType
{
    Undefined, Equal, NotEqual, Less, Greater, LessEqual, GreaterEqual,
};
class Lexem
{
    Lexem(LexemType lexemType_, OperationType operationType_ = OperationType::Undefined, RelationType relationType_ = RelationType::Undefined);
    LexemType GetLexemType() const;
    OperationType GetOperationType() const;
    RelationType GetRelationType() const;
private:
    LexemType m_lexemType;
    OperationType m_opType;
    RelationType m_relationType;
    size_t m_stringNumber;
};


#endif //RGR2_LEXEM_H
