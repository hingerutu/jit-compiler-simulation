#ifndef JIT_COMPILER_H
#define JIT_COMPILER_H

#include "bytecode.h"
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/Support/TargetSelect.h>
#include <memory>

class JITCompiler {
public:
    JITCompiler();
    ~JITCompiler();
    void compileAndExecute(const Bytecode& code);

private:
    std::unique_ptr<llvm::LLVMContext> context_;
    std::unique_ptr<llvm::IRBuilder<>> builder_;
    std::unique_ptr<llvm::Module> module_;
    std::unique_ptr<llvm::orc::LLJIT> jit_;

    void mapOpcodeToIR(const Instruction& instr);
};

#endif // JIT_COMPILER_H
