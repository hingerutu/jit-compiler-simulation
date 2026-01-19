#ifndef BYTECODE_H
#define BYTECODE_H

#include <vector>
#include <variant>

enum Opcode {
    LOAD_CONST,  // Load constant to stack
    ADD,         // Add top two stack elements
    SUB,         // Subtract
    MUL,         // Multiply
    DIV,         // Divide
    JUMP,        // Unconditional jump
    JUMP_IF,     // Jump if top of stack is true
    CALL,        // Call function
    RET,         // Return from function
    PRINT,       // Print top of stack
    HALT         // Stop execution
};

using Operand = std::variant<int, float, std::string>;  // For simplicity, support int, float, string

struct Instruction {
    Opcode op;
    std::vector<Operand> operands;
};

using Bytecode = std::vector<Instruction>;

#endif // BYTECODE_H
