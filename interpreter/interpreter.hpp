#ifndef INTERPRETER_INTERPRETER_HPP
#define INTERPRETER_INTERPRETER_HPP
#include "evaluator.hpp"
#include "parser/parser.hpp"
#include "parser/dumper.hpp"
#include "parser/scanner.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>

namespace TinyC{
    class Interpreter{
    private:
        std::ostream &out;
        Stmt::EvaluateVisitor evaluator;
        Stmt::DumpVisitor dumper{std::cout};
        std::vector<Stmt::StmtObject> statements;
    public:
        Interpreter(std::ostream &out, const std::string &source);
        void interpret();
        void ast_dump();
    };
}

#endif //INTERPRETER_INTERPRETER_HPP
