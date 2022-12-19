#ifndef INTERPRETER_ENVIRONMENT_HPP
#define INTERPRETER_ENVIRONMENT_HPP
#include "parser/token.hpp"
#include "ast/ast_evaluator.hpp"
#include <unordered_map>

namespace TinyC{
    class Environment{
    private:
        Expr::EvaluateVisitor evaluator;
        std::unordered_map<Token::Token, uint64_t> table;
    public:
    };
}


#endif //INTERPRETER_ENVIRONMENT_HPP
