#ifndef INTERPRETER_AST_HPP
#define INTERPRETER_AST_HPP
#include "parser/parser.hpp"
#include <ostream>

namespace TinyC::Token{
    struct DumpVisitor{
        std::ostream &out;
        explicit DumpVisitor(std::ostream &out);
        void operator()();
        void operator()(double value) const;
        void operator()(const std::string &str) const;
    };
}

namespace TinyC::Expr{
    using ExprNode = std::variant<
            std::unique_ptr<Unary>,
            std::unique_ptr<Binary>,
            std::unique_ptr<Variable>,
            std::unique_ptr<Group>,
            std::unique_ptr<Literal>
            >;

    struct DumpVisitor{
        std::ostream &out;
        explicit DumpVisitor(std::ostream &out);
        void operator()(std::unique_ptr<Literal> literalNode);
        void operator()(std::unique_ptr<Unary> unaryNode);
    };
}

#endif //INTERPRETER_AST_HPP
