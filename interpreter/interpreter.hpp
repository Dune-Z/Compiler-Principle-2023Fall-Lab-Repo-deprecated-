#ifndef INTERPRETER_INTERPRETER_HPP
#define INTERPRETER_INTERPRETER_HPP
#include "evaluator.hpp"
#include "environment.hpp"
#include "parser/parser.hpp"
#include "parser/dumper.hpp"
#include "parser/scanner.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>

namespace TinyC{
    class Interpreter{
    private:
        static int working;
        static EnvObject vtable;
        static std::vector<Token::literal_t> fvalue;
        static std::unordered_map<std::string, int> ftable;
        static std::vector<Stmt::StmtObject> statements;


    public:
        explicit Interpreter(const std::string &source);
        void interpret();
        void ast_dump();
    };
}

#endif //INTERPRETER_INTERPRETER_HPP
