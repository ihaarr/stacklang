#ifndef RGR2_LEXER_H
#define RGR2_LEXER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "lexem.h"

//lf == \n
static constexpr char g_newLine = '\n';
class Lexer final
{
public:
    Lexer(const char* filename_);
    const std::vector<Lexem>& GetLexems();
private:
    enum State
    {
        sA1, sA2, sB1, sC1, sD1, sE1, sE2, sE3, sF1, sF2, sF3, sG1, sH1, sI1, sI2, sJ1, sEND,
        sA1a, sA1b, sA2a, sA2b, sA2c, sA2d, sA2e, sA2f, sB1a, sB1b, sC1a, sC1b, sC1c, sC1d, sC1e,
        sC1f, sC1g, sC1h, sD1a, sE1a, sE2a, sE2b, sE3a, sG1a, sG1b, sH1a, sH1b, sI1a, sI2a, sI2b,
        sI2c, sI2d, sM1, sExit1, sExit2, sExit3, sExit4,
        StateCount
    };
    enum Token
    {
        Letter, Digit, Op, Relation, Space, NewLine, Semicolon, Error, End, TokenCount
    };
    struct FindTable
    {
        FindTable(char letter, size_t alternative_, State state_)
        : m_letter(letter), m_alternative(alternative_), m_state(state_) {}
        char m_letter;
        size_t m_alternative;
        State m_state;
    };
public:
    using FuncPtr = State(Lexer::*)();
private:
    void initTable();
    void initFindTable();
    void initBeginVector();
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
    bool m_constFlag = false;
    LexemType m_registerClass;
    char m_currentChar;
    char m_opChar;
    int m_registerNumber;
    int m_regFind;
    char m_registerRelation;
    std::string m_registerVar;
    size_t m_currentNumberStr;
    int* m_registerPointer = nullptr;
    FindTable* m_registerFind = nullptr;

    std::map<std::string, int> m_tableVars;
    std::map<char, int> m_beginVector;
    std::map<int, FindTable> m_tableFind;
    std::vector<Lexem> m_lexems;
    std::vector<int> m_constants;
    FuncPtr table[StateCount][TokenCount];
};


#endif //RGR2_LEXER_H
