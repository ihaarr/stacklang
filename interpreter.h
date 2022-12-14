#ifndef RGR2_INTERPRETER_H
#define RGR2_INTERPRETER_H

#include "virtual_machine.h"

class Interpreter
{
public:
    explicit Interpreter(const char* filename);
    void Start();
private:
    std::string m_filename;
    Lexer m_lexer;
    VirtualMachine m_virtualMachine;
};


#endif //RGR2_INTERPRETER_H
