#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "bytecode.h"
#include "profiler.h"
#include <vector>
#include <stack>
#include <variant>
#include <iostream>

using Value = std::variant<int, float, std::string>;

class Interpreter {
public:
    Interpreter(const Bytecode& code, Profiler& prof);
    void execute();

private:
    Bytecode bytecode_;
    std::stack<Value> stack_;
    size_t pc_;  // Program counter
    Profiler* profiler_;

    void executeInstruction(const Instruction& instr, bool& jumped);
};

#endif // INTERPRETER_H
