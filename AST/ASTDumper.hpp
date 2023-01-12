#ifndef TINYC_ASTDUMPER_HPP
#define TINYC_ASTDUMPER_HPP
#include "ASTVisitor.hpp"
#include "AST.hpp"
#include "Parse/Parser.hpp"
namespace tinyc {
    class ASTDumper: public ASTVisitor {
    private:
        int indent = 0;
        std::vector<ASTNode*> stmts;
        Literal dumpNode(ASTNode* node);
    public:
        ASTDumper() = default;
        explicit ASTDumper(Parser &parser);
        void dump();
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
}
#endif //TINYC_ASTDUMPER_HPP
