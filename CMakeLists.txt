cmake_minimum_required(VERSION 3.23)
project(Interpreter)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include_directories(./)
add_executable(Interpreter
    main.cpp
    parser/token.cpp
    parser/token.hpp
    parser/scanner.cpp
    parser/scanner.hpp common.hpp parser/parser.cpp parser/parser.hpp ast/ast_dumper.cpp ast/ast_dumper.hpp interpreter/interpreter.cpp interpreter/interpreter.hpp ast/ast_traverser.cpp ast/ast_traverser.hpp)

