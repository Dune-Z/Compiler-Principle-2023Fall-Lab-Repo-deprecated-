cmake_minimum_required(VERSION 3.24)
project(TinycCompiler)

set(CMAKE_CXX_STANDARD 17)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.80.0 COMPONENTS program_options)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address 'llvm-config --cxxflags --ldflags --system-libs --libs core'")
find_package(LLVM REQUIRED CONFIG)
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
#link_directories(${LLVM_LIBRARY_DIRS})

include_directories(./)
add_executable(TinycCompiler
        main.cpp
        Lex/Lexer.cpp
        Lex/Lexer.hpp
        Lex/Tokens.cpp
        Lex/Tokens.hpp
        Parse/Parser.cpp
        Parse/Parser.hpp
        AST/AST.cpp
        AST/AST.hpp
        AST/Dumper.cpp
        AST/Dumper.hpp
        AST/Decl.hpp
        AST/Stmt.hpp
        AST/Expr.hpp
        AST/Decl.cpp
        AST/Stmt.cpp
        AST/Expr.cpp
        Parse/ParseDecl.cpp
        Parse/ParseStmt.cpp
        Parse/ParseExpr.cpp
        CodeGen/CodeGen.cpp
        CodeGen/CodeGen.hpp)


if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(TinycCompiler ${Boost_LIBRARIES})
endif()
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif()
if(LLVM_FOUND)
    include_directories(${LLVM_LIBRARY_DIRS})
    target_link_libraries(TinycCompiler ${LLVM_LIBRARIES})
endif()
if(NOT LLVM_FOUND)
    message(FATAL_ERROR "Could not find LLVM!")
endif()
