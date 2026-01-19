
# JIT Compiler Simulation (C++)

This project is an educational simulation of a Just-In-Time (JIT) compiler implemented in C++. It demonstrates how modern runtime systems execute, profile, optimize, and apply JIT-style execution using a simplified bytecode model.

The project focuses on **clarity and learning** rather than being a production-grade compiler.

---

## 📌 Project Overview

The system executes a custom bytecode program using a **stack-based interpreter**, collects runtime execution statistics via a **profiler**, applies **profile-guided optimizations**, and finally executes the optimized bytecode.

An **experimental LLVM-based JIT compiler module** is included for translating optimized bytecode into native machine code when LLVM is available. If LLVM is not installed, the project automatically falls back to executing the optimized bytecode using the interpreter.

---

## 🧠 Architecture & Flow

```
Bytecode
   ↓
Interpreter
   ↓
Profiler (execution counts)
   ↓
Optimizer
   ↓
Optimized Bytecode
   ↓
JIT Execution
   ├── LLVM-based JIT (experimental, optional)
   └── Interpreter fallback (default)
```

---

## 🧩 Key Components

### 1. Bytecode (`bytecode.h`)

* Defines a simple instruction set:

  * `LOAD_CONST`, `ADD`, `SUB`, `MUL`, `DIV`
  * `PRINT`, `JUMP`, `JUMP_IF`, `HALT`
* Instructions consist of an opcode and optional operands.

---

### 2. Interpreter (`interpreter.cpp/.h`)

* Stack-based execution model.
* Executes bytecode instruction by instruction.
* Handles arithmetic, control flow, and output.
* Invokes the profiler during execution.

---

### 3. Profiler (`profiler.cpp/.h`)

* Records how many times each instruction (program counter) is executed.
* Used to identify frequently executed (“hot”) instructions.
* Enables profile-guided optimization.

---

### 4. Optimizer (`optimizer.cpp/.h`)

Applies basic compiler optimizations using profiling data:

* **Constant Folding**
  Pre-computes constant expressions (e.g., `LOAD_CONST 1, LOAD_CONST 2, ADD → LOAD_CONST 3`)
* **Dead Code Elimination**
  Removes instructions whose results are never used.
* Supports saving and loading optimized bytecode from disk.

---

### 5. JIT Compiler (`jit_compiler.cpp/.h`) *(Experimental)*

* Uses LLVM to generate LLVM IR from optimized bytecode.
* Compiles IR to native machine code and executes it.
* Currently supports a limited subset of opcodes.
* Requires LLVM to be installed and configured.

---

### 6. Fallback JIT Execution (`main.cpp`)

* If LLVM is not available, the project **automatically falls back** to executing optimized bytecode using the interpreter.
* Ensures the full pipeline works without external dependencies.

---

## ▶️ Example Bytecode Execution

**Input Bytecode**

```
LOAD_CONST 1
LOAD_CONST 2
ADD
PRINT
HALT
```

**After Optimization**

```
LOAD_CONST 3
PRINT
HALT
```

**Output**

```
3
```

---

## 🛠 Build & Run Instructions

### Option 1: Build without LLVM (recommended)

This runs the interpreter, profiler, optimizer, and simulated JIT.

```bash
mkdir build
cd build
cmake ..
make
./jit_compiler
```

---

### Option 2: Build with LLVM (experimental)

LLVM must be installed and `LLVMConfig.cmake` must be discoverable by CMake.

```bash
cmake -DLLVM_DIR=/path/to/llvm/lib/cmake/llvm ..
make
./jit_compiler
```

---

## 📁 Repository Structure

```
JIT Compiler/
├── bytecode.h
├── interpreter.cpp / .h
├── profiler.cpp / .h
├── optimizer.cpp / .h
├── jit_compiler.cpp / .h
├── main.cpp
├── CMakeLists.txt
├── optimized_code.txt
└── TODO.md
```

---

## ⚠️ Limitations

* Not a production-grade JIT compiler
* LLVM JIT support is experimental and partial
* No dynamic hot-spot recompilation during runtime
* Limited opcode and type support
* No register allocation or advanced optimizations

---

## 🚀 Future Improvements

* Runtime hot-code detection and on-the-fly JIT compilation
* Better LLVM IR stack modeling
* Support for more data types and control flow
* Visualization UI for profiling and optimization
* Advanced optimizations (inlining, loop unrolling)

---

## 🎓 Purpose

This project is intended for:

* Learning compiler and system programming concepts
* Academic coursework and mini-projects
* Demonstrating understanding of JIT compiler pipelines

---

## 📜 License

This project is provided for educational use.

---

If you want, next I can:

* clean this README to be **resume-optimized**, or
* help you **refactor the repo** (remove `build/`, add `.gitignore`, polish structure), or
* write a **short README version** for recruiters.

Just tell me what you want next.
