#ifndef INTERPRETER_EVALUATOR_HPP
#define INTERPRETER_EVALUATOR_HPP
#include "parser/parser.hpp"
#include "environment.hpp"
#include <ostream>
#include <unordered_map>

namespace TinyC::Token{
    struct PrintVisitor{
        void operator()(int value) const;
        void operator()(double value) const;
        void operator()(const std::string &str) const;
    };

    struct UnaryOperationVisitor{
        token_t type;
        explicit UnaryOperationVisitor(const token_t &type);
        literal_t operator()(int value) const;
        literal_t operator()(double value) const;
        literal_t operator()(const std::string &str);
    };

    struct BinaryOperationVisitor{
        token_t type;
        explicit BinaryOperationVisitor(const token_t &type);
        literal_t operator()(const int &lhs, const int &rhs) const;
        literal_t operator()(const int &lhs, const double &rhs) const;
        literal_t operator()(const int &lhs, const std::string &rhs);
        literal_t operator()(const double &lhs, const int &rhs) const;
        literal_t operator()(const double &lhs, const double &rhs) const;
        literal_t operator()(const double &lhs, const std::string &rhs);
        literal_t operator()(const std::string &lhs, const int &rhs);
        literal_t operator()(const std::string &lhs, const double &rhs);
        literal_t operator()(const std::string &lhs, const std::string &rhs) const;
    };
}

namespace TinyC::Expr{
    struct EvaluateVisitor{
        EnvObject environment;
        explicit EvaluateVisitor(EnvObject environment);
        Token::literal_t operator()(std::unique_ptr<Literal> &literalObject);
        Token::literal_t operator()(std::unique_ptr<Variable> &varObject) const;
        Token::literal_t operator()(std::unique_ptr<Call> &callObject);
        Token::literal_t operator()(std::unique_ptr<Unary> &unaryObject);
        Token::literal_t operator()(std::unique_ptr<Binary> &binaryObject);
        Token::literal_t operator()(std::unique_ptr<Group> &groupObject);
    };
}

namespace TinyC::Stmt{
    struct EvaluateVisitor{
        EnvObject environment;
        void operator()(std::unique_ptr<FuncDecl> &funcDeclObject);
        void operator()(std::unique_ptr<VarDecl> &varDeclObject) const;
        void operator()(std::unique_ptr<IfStmt> &ifStmtObject);
        void operator()(std::unique_ptr<WhileStmt> &whileStmtObject);
        void operator()(std::unique_ptr<AssignStmt> &assignStmtObject);
        void operator()(std::unique_ptr<ReturnStmt> &returnStmtObject);
        void operator()(std::unique_ptr<Block> &blockObject);
        void operator()(std::unique_ptr<PrintStmt> &printObject) const;
        void dump_table() const;
    };
}


#endif //INTERPRETER_EVALUATOR_HPP
