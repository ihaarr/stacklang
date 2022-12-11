#ifndef RGR2_LEXER_H
#define RGR2_LEXER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "lexem.h"

static constexpr char g_newLine = '\n';
class Lexer final
{
private:
    enum State
    {
        sA1, sA2, sB1, sC1, sD1, sE1, sE2, sE3, sF1, sF2, sF3, sG1, sH1, sI1, sI2, sJ1, sEND, sM1, StateCount
    };
public:
    using FuncPtr = State (Lexer::*)();
    enum Token
    {
        Letter, Digit, Op, Relation, Space, NewLine, Semicolon, Error, End, TokenCount
    };
    struct FindTable
    {
        FindTable() : m_letter('0'), m_alternative(0), m_stateFunc(nullptr) {}
        FindTable(char letter, int alternative_, FuncPtr stateFunc_)
                : m_letter(letter), m_alternative(alternative_), m_stateFunc(stateFunc_) {}
        char m_letter;
        int m_alternative;
        FuncPtr m_stateFunc;
    };
public:
    Lexer(const char* filename_);
    const std::vector<Lexem>& GetLexems();
private:
    void initTable();
    void initFindTable();
    void initBeginVector();
    void initRelationTable();
    void initOperationTable();
    bool isOp(char);
    bool isRelation(char);
    bool isSemicolon(char);
    bool isNewLine(char);
    bool isEndOfFile(char);
    Token getTokenByChar(char ch);
    void addConst();
    void addVar();
    void createLexem();
    State handleError();
    State A1();
    State A2();
    State B1();
    State C1();
    State D1();
    State E1();
    State E2();
    State E3();
    State F1();
    State F2();
    State F3();
    State G1();
    State H1();
    State I1();
    State I2();
    State J1();
    State A1a();
    State A1b();
    State A2a();
    State A2b();
    State A2c();
    State A2d();
    State A2e();
    State A2f();
    State B1a();
    State B1b();
    State C1a();
    State C1b();
    State C1c();
    State C1d();
    State C1e();
    State C1f();
    State C1g();
    State C1h();
    State D1a();
    State E1a();
    State E2a();
    State E2b();
    State E3a();
    State G1a();
    State G1b();
    State H1a();
    State H1b();
    State I1a();
    State I2a();
    State I2b();
    State I2c();
    State I2d();
    State M1();
    State Exit1();
    State Exit2();
    State Exit3();
    State Exit4();
    void readFileText(std::string&& text_);
private:
    LexemType m_registerClass;
    char m_currentChar;
    int m_registerNumber;
    int m_registerFind;
    char m_registerRelation;
    std::string m_registerVar;
    size_t m_currentNumberStr = 1;
    int* m_registerPointer = nullptr;

    std::map<char, OperationType> m_tableOps;
    std::map<char, int> m_beginVector;
    std::map<int, FindTable> m_tableFind;
    std::map<char, std::map<char, RelationType>> m_relationTable;
    std::vector<Lexem> m_lexems;
    FuncPtr m_table[StateCount][TokenCount];
};


#endif //RGR2_LEXER_H
