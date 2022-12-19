#ifndef INTERPRETER_AST_DUMPER_HPP
#define INTERPRETER_AST_DUMPER_HPP
#include "parser/parser.hpp"
#include <ostream>

namespace TinyC::Token{
    struct DumpVisitor{
        std::ostream &out;
        DumpVisitor(std::ostream &out);
        void operator()();
        void operator()(bool value) const;
        void operator()(int value) const;
        void operator()(double value) const;
        void operator()(const std::string &str) const;
    };
}

namespace TinyC::Expr{
    struct DumpVisitor{
        std::ostream &out;
        explicit DumpVisitor(std::ostream &out);
        void operator()(std::unique_ptr<Literal> &literalObject);
        void operator()(std::unique_ptr<Variable> &varObject);
        void operator()(std::unique_ptr<Unary> &unaryObject);
        void operator()(std::unique_ptr<Binary> &binaryObject);
        void operator()(std::unique_ptr<Group> &groupObject);
    };
}

namespace TinyC::Stmt{
    struct DumpVisitor{
        std::ostream &out;
        explicit DumpVisitor(std::ostream &out);
        void operator()(std::unique_ptr<FuncDecl> &funcDeclObject);
        void operator()(std::unique_ptr<VarDecl> &varDeclObject);
        void operator()(std::unique_ptr<IfStmt> &ifStmtObject);
        void operator()(std::unique_ptr<WhileStmt> &whileStmtObject);
        void operator()(std::unique_ptr<AssignStmt> &assignStmtObject);
        void operator()(std::unique_ptr<ReturnStmt> &returnStmtObject);
        void operator()(std::unique_ptr<Block> &blockObject);
    };
}
#endif //INTERPRETER_AST_DUMPER_HPP
