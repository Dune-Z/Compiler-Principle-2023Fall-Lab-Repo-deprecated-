#ifndef INTERPRETER_INTERPRETER_HPP
#define INTERPRETER_INTERPRETER_HPP
#include "ast/ast_evaluator.hpp"
#include <ostream>
#include <vector>
#include <unordered_map>

namespace TinyC{
    class Interpreter{
    private:
        std::ostream &out;
        Stmt::EvaluateVisitor evaluator;
        std::vector<Stmt::StmtObject> statements;
    public:
        Interpreter(std::ostream &out, std::vector<Stmt::StmtObject> statements);
        void interpret();
    };
}

#endif //INTERPRETER_INTERPRETER_HPP
