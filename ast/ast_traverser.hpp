#ifndef INTERPRETER_AST_TRAVERSER_HPP
#define INTERPRETER_AST_TRAVERSER_HPP
#include "parser/parser.hpp"
namespace TinyC::Expr{
    struct TravelVisitor{
        ExprObject operator()(std::unique_ptr<Unary> unaryObject);
    };
}


#endif //INTERPRETER_AST_TRAVERSER_HPP
