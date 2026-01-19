#include "interpreter.h"

Interpreter::Interpreter(const Bytecode& code, Profiler& prof) : bytecode_(code), pc_(0), profiler_(&prof) {}

void Interpreter::execute() {
    while (pc_ < bytecode_.size()) {
        profiler_->record(pc_);
        bool jumped = false;
        const Instruction& instr = bytecode_[pc_];
        executeInstruction(instr, jumped);
        if (!jumped) ++pc_;
    }
}

void Interpreter::executeInstruction(const Instruction& instr, bool& jumped) {
    jumped = false;
    switch (instr.op) {
        case LOAD_CONST: {
            if (!instr.operands.empty()) {
                stack_.push(instr.operands[0]);
            }
            break;
        }
        case ADD: {
            if (stack_.size() >= 2) {
                auto b = stack_.top(); stack_.pop();
                auto a = stack_.top(); stack_.pop();
                // For simplicity, assume ints
                if (std::holds_alternative<int>(a) && std::holds_alternative<int>(b)) {
                    int result = std::get<int>(a) + std::get<int>(b);
                    stack_.push(result);
                }
            }
            break;
        }
        case PRINT: {
            if (!stack_.empty()) {
                auto val = stack_.top(); stack_.pop();
                if (std::holds_alternative<int>(val)) {
                    std::cout << std::get<int>(val) << std::endl;
                }
            }
            break;
        }
        case JUMP: {
            pc_ = std::get<int>(instr.operands[0]);
            jumped = true;
            break;
        }
        case HALT:
            pc_ = bytecode_.size();  // Exit loop
            jumped = true;
            break;
        default:
            // Unimplemented opcodes
            break;
    }
}
