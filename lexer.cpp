#include <fstream>
#include <iterator>
#include "lexer.h"

Lexer::Lexer()
{
    initBeginVector();
    initFindTable();
    initTable();
    initRelationTable();
    initOperationTable();
}
const std::vector<Lexem> &Lexer::GetLexems()
{
    return m_lexems;
}
void Lexer::Analyze(const char* filename_)
{
    std::ifstream fileStream(filename_);
    if(!fileStream.is_open())
        return;

    std::string text = std::string(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>());
    fileStream.close();
    readFileText(std::move(text));
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
        currentState = (this->*m_table[currentState][token])();
    }
}
void Lexer::initTable()
{
    for(int i = 0; i < StateCount; ++i)
    {
        for(int j = 0; j < TokenCount; ++j)
        {
            m_table[i][j] = &Lexer::error;
        }
    }
    m_table[sA1][Letter] = &Lexer::B1a;
    m_table[sA1][Op] = &Lexer::C1a;
    m_table[sA1][Relation] = &Lexer::D1a;
    m_table[sA1][Space] = &Lexer::A1;
    m_table[sA1][NewLine] = &Lexer::A1b;
    m_table[sA1][Semicolon] = &Lexer::I1a;

    m_table[sA2][Letter] = &Lexer::B1a;
    m_table[sA2][Op] = &Lexer::C1a;
    m_table[sA2][Relation] = &Lexer::D1a;
    m_table[sA2][Space] = &Lexer::A2;
    m_table[sA2][NewLine] = &Lexer::A2a;
    m_table[sA2][Semicolon] = &Lexer::I2a;
    m_table[sA2][End] = &Lexer::Exit1;

    m_table[sB1][Letter] = &Lexer::M1;
    m_table[sB1][NewLine] = &Lexer::A2f;

    m_table[sC1][Space] = &Lexer::C1;
    m_table[sC1][Letter] = &Lexer::C1;
    m_table[sC1][NewLine] = &Lexer::A2;
    m_table[sC1][Semicolon] = &Lexer::I2a;
    m_table[sC1][End] = &Lexer::Exit1;
    m_table[sC1][Digit] = &Lexer::C1;
    m_table[sC1][Op] = &Lexer::C1;
    m_table[sC1][Relation] = &Lexer::C1;

    m_table[sD1][Relation] = &Lexer::C1h;
    m_table[sD1][Space] = &Lexer::C1g;
    m_table[sD1][NewLine] = &Lexer::A2e;
    m_table[sD1][Semicolon] = &Lexer::I2d;
    m_table[sD1][End] = &Lexer::Exit2;

    m_table[sE1][Space] = &Lexer::F1;
    m_table[sE1][NewLine] = &Lexer::A2f;

    m_table[sE2][Space] = &Lexer::F2;
    m_table[sE2][NewLine] = &Lexer::A2f;

    m_table[sE3][Space] = &Lexer::F3;
    m_table[sE3][NewLine] = &Lexer::A2f;

    m_table[sE4][Space] = &Lexer::E4;
    m_table[sE4][NewLine] = &Lexer::E4;

    m_table[sF1][Space] = &Lexer::F1;
    m_table[sF1][NewLine] = &Lexer::A2f;
    m_table[sF1][Letter] = &Lexer::H1a;
    m_table[sF1][Digit] = &Lexer::G1a;
    m_table[sF1][Bracket] = &Lexer::P0;

    m_table[sF2][Space] = &Lexer::F2;
    m_table[sF2][NewLine] = &Lexer::A2f;
    m_table[sF2][Digit] = &Lexer::G1a;
    m_table[sF2][Letter] = &Lexer::F2;

    m_table[sF3][Space] = &Lexer::F3;
    m_table[sF3][NewLine] = &Lexer::A2f;
    m_table[sF3][Letter] = &Lexer::H1a;
    m_table[sF3][Digit] = &Lexer::F3;

    m_table[sG1][Digit] = &Lexer::G1b;
    m_table[sG1][Space] = &Lexer::C1e;
    m_table[sG1][NewLine] = &Lexer::A2c;
    m_table[sG1][Semicolon] = &Lexer::I2b;
    m_table[sG1][End] = &Lexer::Exit3;

    m_table[sH1][Letter] = &Lexer::H1b;
    m_table[sH1][Digit] = &Lexer::H1b;
    m_table[sH1][Space] = &Lexer::C1f;
    m_table[sH1][NewLine] = &Lexer::A2d;
    m_table[sH1][Semicolon] = &Lexer::I2c;
    m_table[sH1][End] = &Lexer::Exit4;

    m_table[sI1][Letter] = &Lexer::I1;
    m_table[sI1][Digit] = &Lexer::I1;
    m_table[sI1][Op] = &Lexer::I1;
    m_table[sI1][Relation] = &Lexer::I1;
    m_table[sI1][Space] = &Lexer::I1;
    m_table[sI1][NewLine] = &Lexer::A1a;
    m_table[sI1][Semicolon] = &Lexer::I1;
    m_table[sI1][Error] = &Lexer::I1;

    m_table[sI2][Letter] = &Lexer::I2;
    m_table[sI2][Digit] = &Lexer::I2;
    m_table[sI2][Op] = &Lexer::I2;
    m_table[sI2][Relation] = &Lexer::I2;
    m_table[sI2][Space] = &Lexer::I2;
    m_table[sI2][NewLine] = &Lexer::A2b;
    m_table[sI2][Semicolon] = &Lexer::I2;
    m_table[sI2][Error] = &Lexer::I2;
    m_table[sI2][End] = &Lexer::Exit1;

    m_table[sJ1][Letter] = &Lexer::J1;
    m_table[sJ1][Digit] = &Lexer::J1;
    m_table[sJ1][Op] = &Lexer::J1;
    m_table[sJ1][Relation] = &Lexer::J1;
    m_table[sJ1][Space] = &Lexer::J1;
    m_table[sJ1][NewLine] = &Lexer::A1b;
    m_table[sJ1][Semicolon] = &Lexer::J1;
    m_table[sJ1][Error] = &Lexer::J1;
    m_table[sJ1][End] = &Lexer::Exit1;

    m_table[sK1][Space] = &Lexer::K1;
    m_table[sK1][Letter] = &Lexer::K1;
    m_table[sK1][Digit] = &Lexer::K1;
    m_table[sK1][Space] = &Lexer::K1;
    m_table[sK1][Op] = &Lexer::K1;
    m_table[sK1][Relation] = &Lexer::K1;
    m_table[sK1][NewLine] = &Lexer::K1;
    m_table[sK1][End] = &Lexer::K1;

    m_table[sK2][Space] = &Lexer::K2;
    m_table[sK3][Space] = &Lexer::K3;

    m_table[sK4][NewLine] = &Lexer::K4;
    m_table[sK4][Space] = &Lexer::K4;

    m_table[sP1][Digit] = &Lexer::P1;
    m_table[sP1][Letter] = &Lexer::P1;

    m_table[sP2][Digit] = &Lexer::P2;
    m_table[sP2][Letter] = &Lexer::P2;

    m_table[sP3][Digit] = &Lexer::P3;
    m_table[sP3][Letter] = &Lexer::P3;
    m_table[sP3][Degree] = &Lexer::P3;

    m_table[sP4][Digit] = &Lexer::P4;
    m_table[sP4][Letter] = &Lexer::P4;

    m_table[sP5][Digit] = &Lexer::P5;
    m_table[sP5][Letter] = &Lexer::P5;
    m_table[sP5][Op] = &Lexer::P5;
    m_table[sP5][Bracket] = &Lexer::P5;
    m_table[sP5][End] = &Lexer::Exit1;
    m_table[sP5][NewLine] = &Lexer::P5;
}
void Lexer::initFindTable()
{
    m_tableFind[0] = FindTable('0', 0, nullptr);
    m_tableFind[1] = FindTable('n', 0, &Lexer::B1b);
    m_tableFind[2] = FindTable('d', 0, &Lexer::C1b);
    m_tableFind[3] = FindTable('i', 4, &Lexer::E2a);
    m_tableFind[4] = FindTable('m', 0, &Lexer::B1b);
    m_tableFind[5] = FindTable('p', 0, &Lexer::E2b);
    m_tableFind[6] = FindTable('o', 8, &Lexer::B1b);
    m_tableFind[7] = FindTable('p', 18, &Lexer::E3a);
    m_tableFind[8] = FindTable('u', 0, &Lexer::B1b);
    m_tableFind[9] = FindTable('s', 0, &Lexer::B1b);
    m_tableFind[10] = FindTable('h', 0, &Lexer::E1a);
    m_tableFind[11] = FindTable('e', 0, &Lexer::B1b);
    m_tableFind[12] = FindTable('a', 0, &Lexer::B1b);
    m_tableFind[13] = FindTable('d', 0, &Lexer::C1c);
    m_tableFind[14] = FindTable('r', 0, &Lexer::B1b);
    m_tableFind[15] = FindTable('i', 0, &Lexer::B1b);
    m_tableFind[16] = FindTable('t', 0, &Lexer::B1b);
    m_tableFind[17] = FindTable('e', 0, &Lexer::C1d);
    m_tableFind[18] = FindTable('l', 0, &Lexer::E4a);
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
void Lexer::initRelationTable()
{
    m_relationTable['=']['='] = RelationType::Undefined;
    m_relationTable['=']['!'] = RelationType::Undefined;
    m_relationTable['=']['<'] = RelationType::Undefined;
    m_relationTable['=']['>'] = RelationType::Undefined;

    m_relationTable['!']['='] = RelationType::NotEqual;
    m_relationTable['!']['!'] = RelationType::Undefined;
    m_relationTable['!']['<'] = RelationType::Undefined;
    m_relationTable['!']['>'] = RelationType::Undefined;

    m_relationTable['<']['='] = RelationType::LessEqual;
    m_relationTable['<']['!'] = RelationType::Less;
    m_relationTable['<']['<'] = RelationType::Less;
    m_relationTable['<']['>'] = RelationType::Less;

    m_relationTable['>']['='] = RelationType::GreaterEqual;
    m_relationTable['>']['!'] = RelationType::Greater;
    m_relationTable['>']['<'] = RelationType::Greater;
    m_relationTable['>']['>'] = RelationType::Greater;
}
void Lexer::initOperationTable()
{
    m_tableOps['+'] = OperationType::Plus;
    m_tableOps['-'] = OperationType::Minus;
    m_tableOps['/'] = OperationType::Div;
    m_tableOps['%'] = OperationType::Mod;
    m_tableOps['*'] = OperationType::Multiply;
}
Lexer::Token Lexer::getTokenByChar(char ch)
{
    if(isdigit(ch)) return Digit;
    if(isalpha(ch)) return Letter;
    if(ch == '^') return Degree;
    if(ch == '[' || ch == ']') return Bracket;
    if(ch == '\0' || ch == EOF) return End;
    if(ch == ';') return Semicolon;
    if(ch == '+' || ch == '-' || ch == '/' || ch == '*' || ch == '%') return Op;
    if(ch == '=' || ch == '!' || ch == '>' || ch == '<') return Relation;
    if(ch == ' ' || ch == '\t') return Space;
    if(ch == g_newLine) return NewLine;
    return Error;
}
void Lexer::addConst()
{
    m_registerClass = LexemType::Constant;
    m_varType = VarType::Int;
}
void Lexer::addVar(VarType type_)
{
    m_registerClass = LexemType::Var;
    m_varType = type_;
}
void Lexer::createLexem()
{
    switch(m_registerClass)
    {
        case LexemType::EndOfFile:
        {
            m_lexems.emplace_back(m_registerClass, m_currentNumberStr, std::string());
            break;
        }
        case LexemType::Op:
        {
            m_lexems.emplace_back(m_registerClass, m_currentNumberStr, std::string(), m_varType, 0, m_tableOps[m_currentChar]);
            break;
        }
        case LexemType::Relation:
        {
            RelationType type = m_relationTable[m_registerRelation][m_registerRelation];
            if(m_relationTable[m_registerRelation].find(m_currentChar) != m_relationTable[m_registerRelation].end())
            {
                type = m_relationTable[m_registerRelation][m_currentChar];
            }
            m_lexems.emplace_back(m_registerClass, m_currentNumberStr, std::string(),  m_varType, 0,
                                  OperationType::Undefined, type);
            break;
        }
        case LexemType::Var:
        {
            m_lexems.emplace_back(m_registerClass, m_currentNumberStr, m_registerVar, m_varType);
            break;
        }
        case LexemType::Constant:
        {
            m_lexems.emplace_back(m_registerClass, m_currentNumberStr, std::string(), m_varType, m_registerNumber);
            break;
        }
        case LexemType::Comment:
        {
            break;
        }
        default:
            m_lexems.emplace_back(m_registerClass, m_currentNumberStr, std::string());
    }
}
Lexer::State Lexer::handleError()
{
    m_registerClass = LexemType::Error;
    std::cout << "Error on line " << m_currentNumberStr << '\n';
    if(m_currentChar == g_newLine)
    {
        ++m_currentNumberStr;
        return sA2;
    }
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
bool Lexer::isSquareOpenBracket(char ch)
{
    return ch == '[';
}
bool Lexer::isSquareCloseBracket(char ch)
{
    return ch == ']';
}
bool Lexer::isSignPlus(char ch)
{
    return ch == '+';
}
Lexer::State Lexer::A1()
{
    if(isNewLine(m_currentChar)) return A1b();
    if(isalpha(m_currentChar)) return B1a();
    if(isOp(m_currentChar)) return C1a();
    if(isRelation(m_currentChar)) return D1a();
    if(isspace(m_currentChar)) return sA1;
    if(isSemicolon(m_currentChar)) return I1a();
    return sEND; //Чтобы не выдавал ошибку
}
Lexer::State Lexer::A2()
{
    if(isalpha(m_currentChar)) return B1a();
    if(isOp(m_currentChar)) return C1a();
    if(isRelation(m_currentChar)) return D1a();
    if(isNewLine(m_currentChar))return A2a();
    if(isspace(m_currentChar)) return sA2;
    if(isSemicolon(m_currentChar)) return I2a();
    return Exit1();
}
Lexer::State Lexer::B1()
{
    if(isalpha(m_currentChar)) return sM1;
    if(isNewLine(m_currentChar)) return A2f();
    return sEND;
}
Lexer::State Lexer::C1()
{
    if(isNewLine(m_currentChar)) return sA2;
    if(isspace(m_currentChar)) return sC1;
    if(isSemicolon(m_currentChar)) return I2a();
    if(isEndOfFile(m_currentChar)) return Exit1();
    m_lexems.pop_back();
    return error();
}
Lexer::State Lexer::D1()
{
    if(isNewLine(m_currentChar)) return A2e();
    if(isRelation(m_currentChar)) return C1h();
    if(isspace(m_currentChar)) return C1g();
    if(isSemicolon(m_currentChar)) return I2d();
    return error();
}
Lexer::State Lexer::E1()
{
    if(isNewLine(m_currentChar)) return A2f();
    if(isspace(m_currentChar))
    {
        createLexem();
        return sF1;
    }
    return error();
}
Lexer::State Lexer::E2()
{
    if(isspace(m_currentChar))
    {
        createLexem();
        return sF2;
    }
    m_lexems.pop_back();
    return error();
}
Lexer::State Lexer::E3()
{
    if(isNewLine(m_currentChar)) return A2f();
    if(isspace(m_currentChar))
    {
        createLexem();
        return sF3;
    }
    return error();
}
Lexer::State Lexer::E4()
{
    if(isNewLine(m_currentChar)) return sA2;
    if(isspace(m_currentChar)) return sE4;
    return error();
}
Lexer::State Lexer::F1()
{
    if(isNewLine(m_currentChar))
    {
        m_lexems.pop_back();
        return A2f();
    }
    if(isalpha(m_currentChar)) return H1a();
    if(isdigit(m_currentChar)) return G1a();
    if(isspace(m_currentChar))
    {
        return sF1;
    }
    m_lexems.pop_back();
    return error();
}
Lexer::State Lexer::F2()
{
    if(isdigit(m_currentChar)) return G1a();
    if(isspace(m_currentChar))
    {
        return sF2;
    }
    m_lexems.pop_back();
    return error();
}
Lexer::State Lexer::F3()
{
    if(isNewLine(m_currentChar))
    {
        m_lexems.pop_back();
        return A2f();
    }
    if(isalpha(m_currentChar)) return H1a();
    if(isspace(m_currentChar)) return sF3;
    m_lexems.pop_back();
    return error();
}
Lexer::State Lexer::K1()
{
    if(isspace(m_currentChar))
    {
        createLexem();
        return sF1;
    }
    if(isSquareOpenBracket(m_currentChar))
    {
        return sP1;
    }
    return error();
}
Lexer::State Lexer::K2()
{
    if(isNewLine(m_currentChar)) return handleError();
    if(isspace(m_currentChar))
    {
        createLexem();
        return sF2;
    }
    return error();
}
Lexer::State Lexer::K3()
{
    if(isspace(m_currentChar))
    {
        createLexem();
        return sF3;
    }
    return error();
}
Lexer::State Lexer::K4()
{
    if(isNewLine(m_currentChar))
    {
        createLexem();
        ++m_currentNumberStr;
        return sA2;
    }
    if(isspace(m_currentChar))
    {
        return sK4;
    }
    return error();
}
Lexer::State Lexer::G1()
{
    if(isNewLine(m_currentChar)) return A2c();
    if(isdigit(m_currentChar)) return G1b();
    if(isspace(m_currentChar)) return C1e();
    if(isSemicolon(m_currentChar)) return I2b();
    return Exit3();
}
Lexer::State Lexer::H1()
{
    if(isNewLine(m_currentChar)) return A2d();
    if(isalpha(m_currentChar)) return H1b();
    if(isdigit(m_currentChar)) return H1b();
    if(isspace(m_currentChar)) return C1f();
    if(isSemicolon(m_currentChar)) return I2c();
    return Exit4();
}
Lexer::State Lexer::I1()
{
    if(isNewLine(m_currentChar)) return A1a();
    return sI1;
}
Lexer::State Lexer::I2()
{
    if(isNewLine(m_currentChar)) return A2b();
    if(isEndOfFile(m_currentChar)) return Exit1();
    return sI2;
}
Lexer::State Lexer::J1()
{
    if(m_currentChar == '\0') return sEND;
    if(isNewLine(m_currentChar)) return A2a();
    if(isEndOfFile(m_currentChar)) return Exit1();
    return sJ1;
}
Lexer::State Lexer::P0()
{
    return sP1;
}
Lexer::State Lexer::P1()
{
    if(isspace(m_currentChar)) return A2f();
    if(isalpha(m_currentChar)) return A2f();
    if(isdigit(m_currentChar)) return P1a();
    return error();
}
Lexer::State Lexer::P2()
{
    if(isspace(m_currentChar)) return A2f();
    if(isalpha(m_currentChar)) return P2b(); //после коэффициента идет буква
    if(isdigit(m_currentChar)) return P2a();
    return error();
}
Lexer::State Lexer::P3() //Встретили х, ждем ^
{
    if(m_currentChar == '^')
    {
        return sP4;
    }
    return error();
}
Lexer::State Lexer::P4() //Встретили ^, начинаем читать степень, если цифра
{
    if(isdigit(m_currentChar))
    {
        m_registerClass = LexemType::Push;
        m_registerNumber = m_currentChar - '0';
        return sP5;
    }
    return error();
}
Lexer::State Lexer::P5() //Продолжаем читать степень.
{
    if(isNewLine(m_currentChar)) return A2a();
    if(isSquareCloseBracket(m_currentChar))
    {
        createLexem();
        addConst();
        createLexem();
        m_registerNumber = m_registerPolCount;
        m_registerClass = LexemType::Push;
        createLexem();
        addConst();
        createLexem();
        m_registerClass = LexemType::Pol;
        createLexem();
        m_registerPolCount = 1;
        return sP5;
    }
    if(isdigit(m_currentChar))
    {
        m_registerNumber = m_registerNumber * 10 + (m_currentChar - '0');
        return sP5;
    }
    if(isSignPlus(m_currentChar))
    {
        ++m_registerPolCount;
        createLexem();
        addConst();
        createLexem();
        m_registerClass = LexemType::Push;
        createLexem();
        return sP1;
    }
    return error();
}
Lexer::State Lexer::P1a() //Обрабатываем коэффициент
{
    m_registerNumber = m_currentChar - '0';
    return sP2;
}
Lexer::State Lexer::P2a()
{
    m_registerNumber = m_registerNumber * 10 + (m_currentChar - '0');
    return sP2;
}
Lexer::State Lexer::P2b()
{
    if(m_currentChar == 'x')
    {
        addConst();
        createLexem();
        m_registerClass = LexemType::Push;
        return sP3;
    }
    return error();
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
        return handleError();
    createLexem();
    ++m_currentNumberStr;
    return sA2;
}
Lexer::State Lexer::A2f()
{
    return handleError();
}
Lexer::State Lexer::B1a()
{
    m_registerFind = m_beginVector[m_currentChar];
    if(m_registerFind == 0)
        return handleError();
    return sB1;
}
Lexer::State Lexer::B1b()
{
    ++m_registerFind;
    return sB1;
}
Lexer::State Lexer::C1a()
{
    m_registerClass = LexemType::Op;
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
        return handleError();
    createLexem();
    return sC1;
}
Lexer::State Lexer::C1h()
{
    RelationType type = m_relationTable[m_registerRelation][m_currentChar];
    if(type == RelationType::Undefined)
        return handleError();
    createLexem();
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
    return sK1;
}
Lexer::State Lexer::E2a()
{
    m_registerClass = LexemType::Ji;
    return sK2;
}
Lexer::State Lexer::E3a()
{
    m_registerClass = LexemType::Pop;
    return sK3;
}
Lexer::State Lexer::E2b()
{
    m_registerClass = LexemType::Jmp;
    return sK2;
}
Lexer::State Lexer::E4a()
{
    m_registerClass = LexemType::Pol;
    return sK4;
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
    m_registerVar.push_back(m_currentChar);
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
    return I1();
}
Lexer::State Lexer::I2a()
{
    m_registerClass = LexemType::Comment;
    return I1();
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
        return handleError();
    createLexem();
    m_registerClass = LexemType::Comment;

    return sI2;
}
Lexer::State Lexer::M1()
{
    while(true)
    {
        auto& table = m_tableFind[m_registerFind];
        auto ptr = table.m_stateFunc;
        if(table.m_letter == m_currentChar)
            return (this->*ptr)();
        else
            m_registerFind = table.m_alternative;
        if(m_registerFind == 0) return handleError();
    }
}
Lexer::State Lexer::Exit1()
{
    m_registerClass = LexemType::EndOfFile;
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
Lexer::State Lexer::error()
{
    return handleError();
}


