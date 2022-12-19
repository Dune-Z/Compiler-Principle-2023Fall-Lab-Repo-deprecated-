#ifndef INTERPRETER_AST_EVALUATOR_HPP
#define INTERPRETER_AST_EVALUATOR_HPP
#include "parser/parser.hpp"
#include <ostream>
#include <unordered_map>

namespace TinyC{
    using table_t = std::unordered_map<std::string, std::pair<Token::literal_t, Token::tokenType>>;
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
    };
}


#endif //INTERPRETER_AST_EVALUATOR_HPP
