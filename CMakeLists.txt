cmake_minimum_required(VERSION 3.10)
project(C-Compiler CXX)

set(SOURCE_FILES
    src/main.cpp
    src/lex.cpp
    src/parser.cpp
    src/ast.cpp
    src/ast_printer.cpp
#    src/codegen.cpp
)

add_executable(test ${SOURCE_FILES})

target_include_directories(test PUBLIC src)

set_property(TARGET test PROPERTY CXX_STANDARD 17)
set_property(TARGET test PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET test PROPERTY CXX_EXTENSIONS OFF)
