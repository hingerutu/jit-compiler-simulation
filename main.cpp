#include "interpreter.h"
#include "profiler.h"
#include "optimizer.h"
#include <iostream>
#include <fstream>

// Minimal JITCompiler stub to avoid requiring LLVM headers during build.
// If you have LLVM available, restore the real "jit_compiler.h" include.
class JITCompiler {
public:
    // Accept the project Bytecode type; this is a no-op fallback.
    void compileAndExecute(const Bytecode& code) {
        std::cout << "JIT compilation skipped (LLVM not available). Executing optimized bytecode via interpreter instead." << std::endl;
        // In absence of LLVM, run the optimized bytecode using the Interpreter as a fallback.
        Profiler prof;
        Interpreter interp(code, prof);
        interp.execute();
    }
};

int main() {
    // Sample bytecode: LOAD_CONST 1, LOAD_CONST 2, ADD, PRINT, HALT
    Bytecode code = {
        {LOAD_CONST, {1}},
        {LOAD_CONST, {2}},
        {ADD, {}},
        {PRINT, {}},
        {HALT, {}}
    };

    Profiler prof;
    // Run once to simulate execution
    Interpreter interp(code, prof);
    interp.execute();

    std::cout << "Execution counts:" << std::endl;
    for (size_t pc = 0; pc < code.size(); ++pc) {
        std::cout << "PC " << pc << ": " << prof.getCount(pc) << std::endl;
    }

    Optimizer opt;
    const std::string cacheFile = "optimized_code.txt";

    Bytecode opt_code;
    std::ifstream cache(cacheFile);
    if (cache.good()) {
        std::cout << "Loading optimized bytecode from cache..." << std::endl;
        opt_code = opt.loadOptimizedBytecode(cacheFile);
        cache.close();
    } else {
        std::cout << "Optimizing bytecode..." << std::endl;
        opt_code = opt.optimize(code, prof);
        opt.saveOptimizedBytecode(opt_code, cacheFile);
    }

    std::cout << "Original code size: " << code.size() << std::endl;
    std::cout << "Optimized code size: " << opt_code.size() << std::endl;

    // Run optimized code
    Profiler prof2;
    Interpreter interp2(opt_code, prof2);
    interp2.execute();

    // Optional: Compile to machine code and execute via JIT
    std::cout << "Compiling to machine code and executing via JIT..." << std::endl;
    JITCompiler jit;
    jit.compileAndExecute(opt_code);

    return 0;
}
