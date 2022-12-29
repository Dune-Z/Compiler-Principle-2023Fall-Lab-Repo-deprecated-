#ifndef INTERPRETER_EVALUATOR_HPP
#define INTERPRETER_EVALUATOR_HPP
#include "parser/parser.hpp"
#include <ostream>
#include <unordered_map>

namespace TinyC{
    using table_t = std::unordered_map<std::string, std::pair<Token::literal_t, Token::token_t>>;
}

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
        table_t table;
        explicit EvaluateVisitor(const table_t &table);
        Token::literal_t operator()(std::unique_ptr<Literal> &literalObject);
        Token::literal_t operator()(std::unique_ptr<Variable> &varObject);
        Token::literal_t operator()(std::unique_ptr<Unary> &unaryObject);
        Token::literal_t operator()(std::unique_ptr<Binary> &binaryObject);
        Token::literal_t operator()(std::unique_ptr<Group> &groupObject);
    };
}

namespace TinyC::Stmt{
    struct EvaluateVisitor{
        table_t table;
        void operator()(std::unique_ptr<FuncDecl> &funcDeclObject);
        void operator()(std::unique_ptr<VarDecl> &varDeclObject);
        void operator()(std::unique_ptr<IfStmt> &ifStmtObject);
        void operator()(std::unique_ptr<WhileStmt> &whileStmtObject);
        void operator()(std::unique_ptr<AssignStmt> &assignStmtObject);
        void operator()(std::unique_ptr<ReturnStmt> &returnStmtObject);
        void operator()(std::unique_ptr<Block> &blockObject);
        void operator()(std::unique_ptr<PrintStmt> &printObject) const;
        void dump_table();
    };
}


#endif //INTERPRETER_EVALUATOR_HPP
