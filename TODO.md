# JIT Compiler Simulation TODO

- [x] Create bytecode.h: Define simple bytecode instruction set (LOAD, ADD, JUMP, CALL, etc.)
- [x] Create interpreter.h and interpreter.cpp: Implement bytecode interpreter
- [x] Create profiler.h and profiler.cpp: Add profiling to count executions
- [x] Create optimizer.h and optimizer.cpp: Implement basic optimizations (constant folding, inlining)
- [x] Create main.cpp: Demonstrate with sample bytecode program
- [x] Compile and run the simulation
- [x] Test with different programs and verify optimizations
- [x] Add caching for optimized bytecode: Save optimized bytecode to optimized_code.txt and load on subsequent runs
- [x] Implement machine code generation: Use LLVM to compile bytecode to actual CPU instructions and execute via JIT
- [x] Update optimizer.h: Add saveOptimizedBytecode() and loadOptimizedBytecode() method declarations
- [x] Update optimizer.cpp: Implement serialization/deserialization for Bytecode to/from text file
- [x] Update main.cpp: Add logic to check for optimized_code.txt, load if exists, else optimize and save; add optional JIT compilation
- [x] Create jit_compiler.h and jit_compiler.cpp: New class for LLVM-based JIT compilation, map opcodes to LLVM IR
- [x] Update CMakeLists.txt: Add LLVM dependency and linking
- [x] Install LLVM if not present (e.g., brew install llvm on macOS)
- [x] Build and test caching functionality
- [x] Build and test machine code generation
- [x] Update TODO.md with completed tasks
