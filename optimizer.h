#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "bytecode.h"
#include "profiler.h"
#include <string>

class Optimizer {
public:
    Bytecode optimize(const Bytecode& code, const Profiler& prof);
    void saveOptimizedBytecode(const Bytecode& code, const std::string& filename);
    Bytecode loadOptimizedBytecode(const std::string& filename);
};

#endif // OPTIMIZER_H
