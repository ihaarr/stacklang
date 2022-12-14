#include "interpreter.h"

Interpreter::Interpreter(const char* filename) : m_filename(filename)
{

}
void Interpreter::Start()
{
    m_lexer.Analyze(m_filename.c_str());
    const auto& lexems = m_lexer.GetLexems();
    for(const auto& lexem : lexems)
    {
        std::cout << lexem << std::endl;
    }
    m_virtualMachine.Run(lexems);
}
