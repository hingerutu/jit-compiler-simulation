#include "optimizer.h"
#include <set>
#include <fstream>
#include <sstream>
#include <iostream>

Bytecode Optimizer::optimize(const Bytecode& code, const Profiler& prof) {
    Bytecode optimized;

    // --- Step 1: Constant Folding (existing logic) ---
    for (size_t i = 0; i < code.size(); ++i) {
        if (i + 2 < code.size() &&
            code[i].op == LOAD_CONST &&
            code[i + 1].op == LOAD_CONST &&
            code[i + 2].op == ADD &&
            prof.isHot(i + 2)) {
            // Fold constants
            int a = std::get<int>(code[i].operands[0]);
            int b = std::get<int>(code[i + 1].operands[0]);
            optimized.push_back({LOAD_CONST, {a + b}});
            i += 2;  // Skip the next two instructions
        } else {
            optimized.push_back(code[i]);
        }
    }

    // --- Step 2: Dead Code Elimination ---
    Bytecode cleaned;
    bool valueUsed = false; // Track if top of stack value is ever used

    // We scan backward to see which instructions’ results are actually used.
    for (int i = (int)optimized.size() - 1; i >= 0; --i) {
        const auto& instr = optimized[i];

        switch (instr.op) {
            case PRINT:
                // PRINT uses the top of the stack
                valueUsed = true;
                cleaned.insert(cleaned.begin(), instr);
                break;

            case ADD:
            case SUB:
            case MUL:
            case DIV:
            case LOAD_CONST:
                if (valueUsed) {
                    // Keep if result contributes to output
                    cleaned.insert(cleaned.begin(), instr);
                    // After using it, assume it was consumed
                    valueUsed = (instr.op != LOAD_CONST);
                } 
                // else: skip it (dead)
                break;

            case HALT:
            case JUMP:
            case JUMP_IF:
                // Control flow instructions are always kept
                cleaned.insert(cleaned.begin(), instr);
                break;

            default:
                // Keep unknown opcodes to be safe
                cleaned.insert(cleaned.begin(), instr);
                break;
        }
    }

    return cleaned;
}

void Optimizer::saveOptimizedBytecode(const Bytecode& code, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return;
    }
    for (const auto& instr : code) {
        file << static_cast<int>(instr.op);
        for (const auto& operand : instr.operands) {
            file << " ";
            if (std::holds_alternative<int>(operand)) {
                file << "int " << std::get<int>(operand);
            } else if (std::holds_alternative<float>(operand)) {
                file << "float " << std::get<float>(operand);
            } else if (std::holds_alternative<std::string>(operand)) {
                file << "string " << std::get<std::string>(operand);
            }
        }
        file << std::endl;
    }
    file.close();
}

Bytecode Optimizer::loadOptimizedBytecode(const std::string& filename) {
    Bytecode code;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for reading." << std::endl;
        return code;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int opInt;
        iss >> opInt;
        Opcode op = static_cast<Opcode>(opInt);
        std::vector<Operand> operands;
        std::string type;
        while (iss >> type) {
            if (type == "int") {
                int val;
                iss >> val;
                operands.push_back(val);
            } else if (type == "float") {
                float val;
                iss >> val;
                operands.push_back(val);
            } else if (type == "string") {
                std::string val;
                iss >> val;
                operands.push_back(val);
            }
        }
        code.push_back({op, operands});
    }
    file.close();
    return code;
}
