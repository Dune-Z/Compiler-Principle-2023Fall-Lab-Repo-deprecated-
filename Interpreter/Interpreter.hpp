#ifndef TINYC_INTERPRETER_HPP
#define TINYC_INTERPRETER_HPP
#include "AST/AST.hpp"
#include "AST/ASTVisitor.hpp"
#include "Parse/Parser.hpp"
#include "Environment.hpp"
#include <vector>

namespace tinyc {
    class Interpreter: public ASTVisitor {
    private:
        bool atReturn = false;
        Environment environment;
        std::vector<ASTNode*> stmts;
        Literal evaluate(ASTNode *node);
    public:
        explicit Interpreter(Parser &parser);
        Literal interpret();
        Literal visit(LiteralExpr* exprNode) override;
        Literal visit(VarExpr* exprNode) override;
        Literal visit(CallExpr* exprNode) override;
        Literal visit(UnaryExpr* exprNode) override;
        Literal visit(BinaryExpr* exprNode) override;
        Literal visit(GroupExpr* exprNode) override;

        Literal visit(FuncDeclStmt* stmtNode) override;
        Literal visit(VarDeclStmt* stmtNode) override;
        Literal visit(AssignStmt* stmtNode) override;
        Literal visit(IfStmt* stmtNode) override;
        Literal visit(WhileStmt* stmtNode) override;
        Literal visit(BlockStmt* stmtNode) override;
        Literal visit(ReturnStmt* stmtNode) override;
        Literal visit(PrintStmt* stmtNode) override;
    };

    struct UnaryOperationVisitor{
        TokenKind type;
        explicit UnaryOperationVisitor(const TokenKind &type);
        Literal operator()(int value) const;
        Literal operator()(float value) const;
        Literal operator()(const std::string &str);
    };

    struct BinaryOperationVisitor{
        TokenKind type;
        explicit BinaryOperationVisitor(const TokenKind &type);
        Literal operator()(const int &lhs, const int &rhs) const;
        Literal operator()(const int &lhs, const float &rhs) const;
        Literal operator()(const int &lhs, const std::string &rhs);
        Literal operator()(const float &lhs, const int &rhs) const;
        Literal operator()(const float &lhs, const float &rhs) const;
        Literal operator()(const float &lhs, const std::string &rhs);
        Literal operator()(const std::string &lhs, const int &rhs);
        Literal operator()(const std::string &lhs, const float &rhs);
        Literal operator()(const std::string &lhs, const std::string &rhs) const;
    };

}

#endif //TINYC_INTERPRETER_HPP
