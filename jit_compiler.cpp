#include "jit_compiler.h"
#include <iostream>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

JITCompiler::JITCompiler() {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    context_ = std::make_unique<llvm::LLVMContext>();
    module_ = std::make_unique<llvm::Module>("JITModule", *context_);
    builder_ = std::make_unique<llvm::IRBuilder<>>(*context_);

    auto jit = llvm::orc::LLJITBuilder().create();
    if (!jit) {
        std::cerr << "Failed to create LLJIT" << std::endl;
        exit(1);
    }
    jit_ = std::move(*jit);

    // Add printf function declaration
    auto printfType = llvm::FunctionType::get(llvm::Type::getInt32Ty(*context_), llvm::PointerType::get(*context_, 0), true);
    printfFunc_ = llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, "printf", *module_);
}

JITCompiler::~JITCompiler() = default;

void JITCompiler::compileAndExecute(const Bytecode& code) {
    // Create a function type: void () - no args, no return
    auto funcType = llvm::FunctionType::get(llvm::Type::getVoidTy(*context_), false);
    auto func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "mainFunc", *module_);

    // Create entry block
    auto entry = llvm::BasicBlock::Create(*context_, "entry", func);
    builder_->SetInsertPoint(entry);

    // Stack simulation: use a vector of i32 for simplicity (assuming int values)
    std::vector<llvm::Value*> stack;

    for (const auto& instr : code) {
        mapOpcodeToIR(instr, stack);
    }

    // End function
    builder_->CreateRetVoid();

    // Verify and compile
    if (llvm::verifyModule(*module_, &llvm::errs())) {
        std::cerr << "Module verification failed" << std::endl;
        return;
    }

    // Use the original context for the module
    auto err = jit_->addIRModule(llvm::orc::ThreadSafeModule(std::move(module_), std::move(context_)));
    if (err) {
        std::cerr << "Failed to add module to JIT" << std::endl;
        return;
    }

    // Lookup and execute
    auto sym = jit_->lookup("mainFunc");
    if (!sym) {
        std::cerr << "Failed to find symbol" << std::endl;
        return;
    }
    auto addr = sym->getValue();
    auto funcPtr = reinterpret_cast<void (*)()>(addr);
    funcPtr();

    std::cout << "JIT execution completed." << std::endl;
}

void JITCompiler::mapOpcodeToIR(const Instruction& instr, std::vector<llvm::Value*>& stack) {
    switch (instr.op) {
        case LOAD_CONST: {
            if (instr.operands.empty()) break;
            int val = std::get<int>(instr.operands[0]);
            auto constVal = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_), val);
            stack.push_back(constVal);
            break;
        }
        case ADD: {
            if (stack.size() < 2) {
                std::cerr << "Stack underflow in ADD" << std::endl;
