#ifndef RGR2_LEXEM_H
#define RGR2_LEXEM_H

#include <cstdio>
#include <string>

enum class VarType
{
    Int, MyType
};
enum class LexemType
{
    Constant, Var, Push, Pop, Op, Relation, Jmp, Ji, Read, Write, End, Comment, Error, Pol, EndOfFile
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
public:
    Lexem(LexemType lexemType_, size_t strNum_, const std::string& varName_, VarType varType = VarType::Int, int constValue_ = 0,
          OperationType operationType_ = OperationType::Undefined, RelationType relationType_ = RelationType::Undefined);
    LexemType GetLexemType() const;
    VarType GetVarType() const;
    OperationType GetOperationType() const;
    RelationType GetRelationType() const;
    std::string GetVarName() const;
    std::string ToString() const;
    size_t GetStringNumber() const;
    size_t GetConstValue() const;
    friend std::ostream& operator<<(std::ostream& os_, const Lexem& lexem_);
private:
    std::string getOperationInfo() const;
    std::string getRelationInfo() const;
private:
    LexemType m_lexemType;
    VarType m_varType;
    size_t m_stringNumber;
    std::string m_varName;
    size_t m_constValue;
    OperationType m_opType;
    RelationType m_relationType;
};


#endif //RGR2_LEXEM_H
