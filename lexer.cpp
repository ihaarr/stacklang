#include <fstream>
#include <iterator>
#include "lexer.h"


Lexer::Lexer(const char *filename_)
{
    std::ifstream fileStream(filename_);
    if(!fileStream.is_open())
        return;

    initBeginVector();
    initFindTable();
    initTable();
    std::string text = std::string(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>());
    fileStream.close();
    readFileText(std::move(text));
}
const std::vector<Lexem> &Lexer::GetLexems()
{

    return {};
}
void Lexer::readFileText(std::string&& text_)
{
    if(text_.empty())
        return;

    State currentState = State::sA1;
    size_t currentPos = 0;
    while(currentState != sEND)
    {
        m_currentChar = text_[currentPos++];
        Token token = getTokenByChar(m_currentChar);
        currentState = (this->*table[currentState][token])();
    }
}
void Lexer::initTable()
{
    for(int i = 0; i < StateCount; ++i)
    {
        for(int j = 0; j < TokenCount; ++j)
        {
            table[i][j] = &Lexer::E1;
        }
    }
    table[sA1][Letter] = &Lexer::B1a;
    table[sA1][Op] = &Lexer::C1a;
    table[sA1][Relation] = &Lexer::D1a;
    table[sA1][Space] = &Lexer::A1;
    table[sA1][NewLine] = &Lexer::I1a;

    table[sA2][Letter] = &Lexer::B1a;
    table[sA2][Op] = &Lexer::C1a;
    table[sA2][Relation] = &Lexer::D1a;
    table[sA2][Space] = &Lexer::A2;
    table[sA2][NewLine] = &Lexer::A2a;

    table[sB1][Letter] = &Lexer::M1;
    table[sB1][NewLine] = &Lexer::A2f;

    table[sC1][Space] = &Lexer::C1;
    table[sC1][NewLine] = &Lexer::A2;
    table[sC1][Semicolon] = &Lexer::I2a;
    table[sC1][End] = &Lexer::Exit1;

    table[sD1][Relation] = &Lexer::C1h;
    table[sD1][Space] = &Lexer::C1g;
    table[sD1][NewLine] = &Lexer::A2e;
    table[sD1][Semicolon] = &Lexer::I2d;
    table[sD1][End] = &Lexer::Exit2;

    table[sE1][Space] = &Lexer::F1;
    table[sE1][NewLine] = &Lexer::A2f;

    table[sE2][Space] = &Lexer::F2;
    table[sE2][NewLine] = &Lexer::A2f;

    table[sE3][Space] = &Lexer::F3;
    table[sE3][NewLine] = &Lexer::A2f;

    table[sF1][Space] = &Lexer::F1;
    table[sF1][NewLine] = &Lexer::A2f;
    table[sF1][Letter] = &Lexer::H1a;
    table[sF1][Digit] = &Lexer::G1a;

    table[sF2][Space] = &Lexer::F2;
    table[sF2][NewLine] = &Lexer::A2f;
    table[sF2][Digit] = &Lexer::G1a;

    table[sF3][Space] = &Lexer::F3;
    table[sF3][NewLine] = &Lexer::A2f;
    table[sF3][Letter] = &Lexer::H1a;

    table[sG1][Digit] = &Lexer::G1b;
    table[sG1][Space] = &Lexer::C1e;
    table[sG1][NewLine] = &Lexer::A2c;
    table[sG1][Semicolon] = &Lexer::I2b;
    table[sG1][End] = &Lexer::Exit3;

    table[sH1][Letter] = &Lexer::H1b;
    table[sH1][Digit] = &Lexer::H1b;
    table[sH1][Space] = &Lexer::C1f;
    table[sH1][NewLine] = &Lexer::A2d;
    table[sH1][Semicolon] = &Lexer::I2c;
    table[sH1][End] = &Lexer::Exit4;

    table[sI1][Letter] = &Lexer::I1;
    table[sI1][Digit] = &Lexer::I1;
    table[sI1][Op] = &Lexer::I1;
    table[sI1][Relation] = &Lexer::I1;
    table[sI1][Space] = &Lexer::I1;
    table[sI1][NewLine] = &Lexer::A1a;
    table[sI1][Semicolon] = &Lexer::I1;
    table[sI1][Error] = &Lexer::I1;

    table[sI2][Letter] = &Lexer::I2;
    table[sI2][Digit] = &Lexer::I2;
    table[sI2][Op] = &Lexer::I2;
    table[sI2][Relation] = &Lexer::I2;
    table[sI2][Space] = &Lexer::I2;
    table[sI2][NewLine] = &Lexer::A2b;
    table[sI2][Semicolon] = &Lexer::I2;
    table[sI2][Error] = &Lexer::I2;
    table[sI2][End] = &Lexer::Exit1;

    table[sJ1][Letter] = &Lexer::J1;
    table[sJ1][Digit] = &Lexer::J1;
    table[sJ1][Op] = &Lexer::J1;
    table[sJ1][Relation] = &Lexer::J1;
    table[sJ1][Space] = &Lexer::J1;
    table[sJ1][NewLine] = &Lexer::A1b;
    table[sJ1][Semicolon] = &Lexer::J1;
    table[sJ1][Error] = &Lexer::J1;
    table[sJ1][End] = &Lexer::Exit1;
}
void Lexer::initFindTable()
{
    m_tableFind[0] = FindTable('0', 0, State::sEND);
    m_tableFind[1] = FindTable('n', 0, State::sB1b);
    m_tableFind[2] = FindTable('d', 0, State::sC1b);
    m_tableFind[3] = FindTable('i', 4, State::sE2a);
    m_tableFind[4] = FindTable('m', 0, State::sB1b);
    m_tableFind[5] = FindTable('p', 0, State::sE2b);
    m_tableFind[6] = FindTable('o', 8, State::sB1b);
    m_tableFind[7] = FindTable('p', 0, State::sE3a);
    m_tableFind[8] = FindTable('u', 0, State::sB1b);
    m_tableFind[9] = FindTable('s', 0, State::sB1b);
    m_tableFind[10] = FindTable('h', 0, State::sE1a);
    m_tableFind[11] = FindTable('e', 0, State::sB1b);
    m_tableFind[12] = FindTable('s', 0, State::sB1b);
    m_tableFind[13] = FindTable('d', 0, State::sC1c);
    m_tableFind[14] = FindTable('r', 0, State::sB1b);
    m_tableFind[15] = FindTable('i', 0, State::sB1b);
    m_tableFind[16] = FindTable('t', 0, State::sB1b);
    m_tableFind[17] = FindTable('e', 0, State::sC1d);
}
void Lexer::initBeginVector()
{
    m_beginVector['a'] = 0;
    m_beginVector['b'] = 0;
    m_beginVector['c'] = 0;
    m_beginVector['d'] = 0;
    m_beginVector['f'] = 0;
    m_beginVector['g'] = 0;
    m_beginVector['h'] = 0;
    m_beginVector['i'] = 0;
    m_beginVector['k'] = 0;
    m_beginVector['l'] = 0;
    m_beginVector['m'] = 0;
    m_beginVector['n'] = 0;
    m_beginVector['o'] = 0;
    m_beginVector['q'] = 0;
    m_beginVector['s'] = 0;
    m_beginVector['t'] = 0;
    m_beginVector['u'] = 0;
    m_beginVector['v'] = 0;
    m_beginVector['x'] = 0;
    m_beginVector['y'] = 0;
    m_beginVector['z'] = 0;
    m_beginVector['e'] = 1;
    m_beginVector['j'] = 3;
    m_beginVector['p'] = 6;
    m_beginVector['r'] = 11;
    m_beginVector['w'] = 14;
}
Lexer::Token Lexer::getTokenByChar(char ch)
{
    if(isdigit(ch)) return Digit;
    if(isalpha(ch)) return Letter;
    if(ch == ';') return Semicolon;
    if(ch == '+' || ch == '-' || ch == '/' || ch == '*' || ch == '%') return Op;
    if(ch == '=' || ch == '!' || ch == '>' || ch == '<') return Relation;
    if(ch == ' ' || ch == '\t') return Space;
    if(ch == g_newLine) return NewLine;
    return Error;
}
void Lexer::addConst()
{
    if(!m_constFlag) return;
    m_constants.push_back(m_registerNumber);
    m_registerPointer = &m_constants[m_constants.size() - 1];
}
void Lexer::addVar()
{
    m_tableVars[m_registerVar] = m_registerNumber;
    m_registerPointer = &m_tableVars[m_registerVar];
}
void Lexer::createLexem()
{

}
Lexer::State Lexer::handleError()
{
    m_registerClass = LexemType::Error;
    createLexem();
    std::cout << "Error on line " << m_currentNumberStr << '\n';
    return sJ1;
}
bool Lexer::isRelation(char ch)
{
    return ch == '>' || ch == '<' || ch == '=' || ch == '!';
}
bool Lexer::isSemicolon(char ch)
{
    return ch == ';';
}
bool Lexer::isNewLine(char ch)
{
    return ch == g_newLine;
}
bool Lexer::isOp(char ch)
{
    return ch == '+' || ch == '-' || ch == '/' || ch == '%' || ch == '*';
}
bool Lexer::isEndOfFile(char ch)
{
    return ch == EOF;
}

Lexer::State Lexer::A1()
{
    if(isalpha(m_currentChar)) return sB1a;
    if(isOp(m_currentChar)) return sC1a;
    if(isRelation(m_currentChar)) return sD1a;
    if(isspace(m_currentChar)) return sA1;
    if(isNewLine(m_currentChar)) return sA1b;
    if(isSemicolon(m_currentChar)) return sI1a;
    return sEND; //Чтобы не выдавал ошибку
}
Lexer::State Lexer::A2()
{
    if(isalpha(m_currentChar)) return sB1a;
    if(isOp(m_currentChar)) return sC1a;
    if(isRelation(m_currentChar)) return sD1a;
    if(isspace(m_currentChar)) return sA2;
    if(isNewLine(m_currentChar)) return sA2a;
    if(isSemicolon(m_currentChar)) return sI2a;
    return sExit1;
}
Lexer::State Lexer::B1()
{
    if(isalpha(m_currentChar)) return sM1;
    if(isNewLine(m_currentChar)) return sA2f;
    return sEND;
}
Lexer::State Lexer::C1()
{
    if(isspace(m_currentChar)) return sC1;
    if(isNewLine(m_currentChar)) return sA2;
    if(isSemicolon(m_currentChar)) return sI2a;
    if(isEndOfFile(m_currentChar)) return sExit1;
    return sEND;
}
Lexer::State Lexer::D1()
{
    if(isRelation(m_currentChar)) return sC1h;
    if(isspace(m_currentChar)) return sC1g;
    if(isNewLine(m_currentChar)) return sA2e;
    if(isSemicolon(m_currentChar)) return sI2d;
    return sEND;
}
Lexer::State Lexer::E1()
{
    if(isspace(m_currentChar)) return sF1;
    if(isNewLine(m_currentChar)) return sA2f;
    return sEND;
}
Lexer::State Lexer::E2()
{
    if(isspace(m_currentChar)) return sF2;
    if(isNewLine(m_currentChar)) return sA2f;
    return sEND;
}
Lexer::State Lexer::E3()
{
    if(isspace(m_currentChar)) return sF3;
    if(isNewLine(m_currentChar)) return sA2f;
    return sEND;
}
Lexer::State Lexer::F1()
{
    if(isalpha(m_currentChar)) return sH1a;
    if(isdigit(m_currentChar)) return sG1a;
    if(isspace(m_currentChar)) return sF1;
    if(isNewLine(m_currentChar)) return sA2f;
    return sEND;
}
Lexer::State Lexer::F2()
{
    if(isdigit(m_currentChar)) return sG1a;
    if(isspace(m_currentChar)) return sF2;
    if(isNewLine(m_currentChar)) return sA2f;
    return sEND;
}
Lexer::State Lexer::F3()
{
    if(isalpha(m_currentChar)) return sH1a;
    if(isspace(m_currentChar)) return sF3;
    if(isNewLine(m_currentChar)) return sA2f;
    return sEND;
}
Lexer::State Lexer::G1()
{
    if(isdigit(m_currentChar)) return sG1b;
    if(isspace(m_currentChar)) return sC1e;
    if(isNewLine(m_currentChar)) return sA2c;
    if(isSemicolon(m_currentChar)) return sI2b;
    return sExit3;
}
Lexer::State Lexer::H1()
{
    if(isalpha(m_currentChar)) return sH1b;
    if(isdigit(m_currentChar)) return sH1b;
    if(isspace(m_currentChar)) return sC1f;
    if(isNewLine(m_currentChar)) return sA2d;
    if(isSemicolon(m_currentChar)) return sI2c;
    return sExit4;
}
Lexer::State Lexer::I1()
{
    if(isNewLine(m_currentChar)) return sA1a;
    return sI1;
}
Lexer::State Lexer::I2()
{
    if(isNewLine(m_currentChar)) return sA2b;
    if(isEndOfFile(m_currentChar)) return sExit1;
    return sI2;
}
Lexer::State Lexer::J1()
{
    if(isNewLine(m_currentChar)) return sA2a;
    if(isEndOfFile(m_currentChar)) return sExit1;
    return sJ1;
}
Lexer::State Lexer::A1a()
{
    createLexem();
    ++m_currentNumberStr;
    return sA1;
}
Lexer::State Lexer::A1b()
{
    ++m_currentNumberStr;
    return sA1;
}
Lexer::State Lexer::A2a()
{
    ++m_currentNumberStr;
    return sA2;
}
Lexer::State Lexer::A2b()
{
    createLexem();
    ++m_currentNumberStr;
    return sA2;
}
Lexer::State Lexer::A2c()
{
    addConst();
    createLexem();
    ++m_currentNumberStr;
    return sA2;
}
Lexer::State Lexer::A2d()
{
    addVar();
    createLexem();
    ++m_currentNumberStr;;
    return sA2;
}
Lexer::State Lexer::A2e()
{
    if(m_registerRelation == '!')
        handleError();
    createLexem();
    ++m_currentNumberStr;
    return sA2;
}
Lexer::State Lexer::A2f()
{
    if(m_registerClass == LexemType::Error)
        handleError();
    createLexem();
    ++m_currentNumberStr;
    return sA2;
}
Lexer::State Lexer::B1a()
{
    m_registerFind = &m_tableFind[m_beginVector[m_currentChar]];
    // if = 0 handleError() if(*m_registerFind)
    return sB1a;
}
Lexer::State Lexer::B1b()
{
    return sB1;
}
Lexer::State Lexer::C1a()
{
    m_registerClass = LexemType::Op;
    m_opChar = m_currentChar;
    createLexem();
    return sC1;
}
Lexer::State Lexer::C1b()
{
    m_registerClass = LexemType::End;
    createLexem();
    return sC1;
}
Lexer::State Lexer::C1c()
{
    m_registerClass = LexemType::Read;
    createLexem();
    return sC1;
}
Lexer::State Lexer::C1d()
{
    m_registerClass = LexemType::Write;
    createLexem();
    return sC1;
}
Lexer::State Lexer::C1e()
{
    addConst();
    createLexem();
    return sC1;
}
Lexer::State Lexer::C1f()
{
    addVar();
    createLexem();
    return sC1;
}
Lexer::State Lexer::C1g()
{
    if(m_registerRelation == '!')
        handleError();
    createLexem();
    return sC1;
}
Lexer::State Lexer::C1h()
{
    //todo
    return sC1;
}
Lexer::State Lexer::D1a()
{
    m_registerClass = LexemType::Relation;
    m_registerRelation = m_currentChar;
    return sD1;
}
Lexer::State Lexer::E1a()
{
    m_registerClass = LexemType::Push;
    m_constFlag = true;
    return sE1;
}
Lexer::State Lexer::E2a()
{
    m_registerClass = LexemType::Push;
    m_constFlag = false;
    return sE2;
}
Lexer::State Lexer::E3a()
{
    m_registerClass = LexemType::Pop;
    return sE3;
}
Lexer::State Lexer::E2b()
{
    m_registerClass = LexemType::Jmp;
    m_constFlag = false;
    return sE2;
}
Lexer::State Lexer::G1a()
{
    m_registerNumber = m_currentChar - '0';
    return sG1;
}
Lexer::State Lexer::G1b()
{
    m_registerNumber = m_registerNumber * 10 + (m_currentChar - '0');
    return sG1;
}
Lexer::State Lexer::H1a()
{
    m_registerVar.clear();
    m_registerVar[0] = m_currentChar;
    return sH1;
}
Lexer::State Lexer::H1b()
{
    m_registerVar.push_back(m_currentChar);
    return sH1;
}
Lexer::State Lexer::I1a()
{
    m_registerClass = LexemType::Comment;
    return sI1;
}
Lexer::State Lexer::I2a()
{
    m_registerClass = LexemType::Comment;
    return sI1;
}
Lexer::State Lexer::I2b()
{
    addConst();
    createLexem();
    m_registerClass = LexemType::Comment;
    return sI2;
}
Lexer::State Lexer::I2c()
{
    addVar();
    createLexem();
    m_registerClass = LexemType::Comment;
    return sI2;
}
Lexer::State Lexer::I2d()
{
    if(m_registerRelation == '!')
        handleError();
    createLexem();
    m_registerClass = LexemType::Comment;
    return sI2;
}
Lexer::State Lexer::M1()
{
    return sM1;
}
Lexer::State Lexer::Exit1()
{
    m_registerClass = LexemType::End;
    createLexem();
    return sEND;
}
Lexer::State Lexer::Exit2()
{
    if(m_registerRelation == '!')
        m_registerClass = LexemType::Error;
    createLexem();
    m_registerClass = LexemType::End;
    createLexem();
    return sEND;
}
Lexer::State Lexer::Exit3()
{
    addConst();
    createLexem();
    m_registerClass = LexemType::End;
    createLexem();
    return sEND;
}
Lexer::State Lexer::Exit4()
{
    addVar();
    createLexem();
    m_registerClass = LexemType::End;
    createLexem();
    return sEND;
}

