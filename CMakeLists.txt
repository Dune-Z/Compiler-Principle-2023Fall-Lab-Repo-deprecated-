cmake_minimum_required(VERSION 3.24)
project(tinyc)

set(CMAKE_CXX_STANDARD 17)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.80.0 COMPONENTS program_options)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
include_directories(./)

add_executable(tinyc
        main.cpp
        Lex/Token.cpp
        Lex/Token.hpp
        Lex/Lexer.cpp
        Lex/Lexer.hpp
        Parse/Parser.cpp
        Parse/Parser.hpp
        Parse/ParseStmt.cpp
        Parse/ParseExpr.cpp
        Interpreter/Interpreter.cpp
        Interpreter/Interpreter.hpp
        Interpreter/Environment.cpp
        Interpreter/Environment.hpp
        Interpreter/InterpretStmt.cpp
        Interpreter/InterpretExpr.cpp
        AST/AST.cpp
        AST/AST.hpp
        AST/ASTDumper.cpp
        AST/ASTDumper.hpp
        AST/ASTVisitor.cpp
        AST/ASTVisitor.hpp
        CodeGen/CGExpr.cpp CodeGen/CGStmt.cpp)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(tinyc ${Boost_LIBRARIES})
endif()
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif()
