#ifndef TINYC_ASTVISITOR_HPP
#define TINYC_ASTVISITOR_HPP
#include "AST.hpp"
namespace tinyc {
    struct LiteralExpr;
    struct VarExpr;
    struct CallExpr;
    struct UnaryExpr;
    struct BinaryExpr;
    struct GroupExpr;
    struct FuncDeclStmt;
    struct VarDeclStmt;
    struct AssignStmt;
    struct IfStmt;
    struct WhileStmt;
    struct BlockStmt;
    struct ReturnStmt;
    struct PrintStmt;

    class ASTVisitor {
    private:
    public:
        ASTVisitor() = default;
        virtual ~ASTVisitor() = default;

        virtual Literal visit(LiteralExpr* exprNode) = 0;
        virtual Literal visit(VarExpr* exprNode) = 0;
        virtual Literal visit(CallExpr* exprNode) = 0;
        virtual Literal visit(UnaryExpr* exprNode) = 0;
        virtual Literal visit(BinaryExpr* exprNode) = 0;
        virtual Literal visit(GroupExpr* exprNode) = 0;

        virtual Literal visit(FuncDeclStmt* stmtNode) = 0;
        virtual Literal visit(VarDeclStmt* stmtNode) = 0;
        virtual Literal visit(AssignStmt* stmtNode) = 0;
        virtual Literal visit(IfStmt* stmtNode) = 0;
        virtual Literal visit(WhileStmt* stmtNode) = 0;
        virtual Literal visit(BlockStmt* stmtNode) = 0;
        virtual Literal visit(ReturnStmt* stmtNode) = 0;
        virtual Literal visit(PrintStmt* stmtNode) = 0;
    };
}

#endif //TINYC_ASTVISITOR_HPP
