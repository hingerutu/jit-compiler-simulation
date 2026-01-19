cmake_minimum_required(VERSION 3.10)
project(JITCompiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Point CMake to your Homebrew LLVM install
set(LLVM_DIR "/opt/homebrew/opt/llvm/lib/cmake/llvm")

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Ensure CMake uses Homebrew’s Clang (optional but recommended)
set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")
set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")

add_executable(jit_compiler
    main.cpp
    interpreter.cpp
    profiler.cpp
    optimizer.cpp
    jit_compiler.cpp
)

llvm_map_components_to_libnames(llvm_libs core orcjit native)
target_link_libraries(jit_compiler ${llvm_libs})

target_include_directories(jit_compiler PRIVATE .)
