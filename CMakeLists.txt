cmake_minimum_required(VERSION 3.23)
project(Interpreter)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.80.0 COMPONENTS program_options)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include_directories(./)
add_executable(Interpreter
        main.cpp
        parser/token.cpp
        parser/token.hpp
        parser/scanner.cpp
        parser/scanner.hpp
        parser/parser.cpp
        parser/parser.hpp
        parser/dumper.cpp
        parser/dumper.hpp
        interpreter/interpreter.cpp
        interpreter/interpreter.hpp
        interpreter/environment.cpp
        interpreter/environment.hpp
        interpreter/evaluator.cpp
        interpreter/evaluator.hpp
        )


if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(Interpreter ${Boost_LIBRARIES})
endif()
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif()

