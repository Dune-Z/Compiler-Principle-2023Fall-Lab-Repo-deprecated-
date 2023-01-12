#ifndef TINYC_AST_HPP
#define TINYC_AST_HPP
#include "Lex/Token.hpp"
#include "ASTVisitor.hpp"
#include <vector>

namespace tinyc{
    class ASTVisitor;
    enum ASTNodeType {
        BaseNode,

        LiteralExprNode, VarExprNode, CallExprNode,
        UnaryExprNode, BinaryExprNode, GroupExprNode,

        FuncDeclStmtNode, VarDeclStmtNode, AssignStmtNode,
        IfStmtNode, WhileStmtNode, BlockStmtNode, ReturnStmtNode,
        PrintStmtNode,
    };

    class ASTNode{
    public:
        ASTNode() = default;
        virtual ~ASTNode() = default;
        virtual ASTNodeType nodeType() {return BaseNode;}
        virtual Literal accept(ASTVisitor& visitor) = 0;
    };
    using Stmt = ASTNode;
    using Expr = ASTNode;
    using StmtPtr = Stmt*;
    using ExprPtr = Expr*;
    using TypeTokenPair = std::pair<TokenKind, Token>;

    struct LiteralExpr: public Expr {
        Token literal;
        explicit LiteralExpr(Token literal): literal(std::move(literal)) {}
        Literal accept(ASTVisitor& visitor) override;
        ASTNodeType nodeType() override {return LiteralExprNode;}
    };

    struct VarExpr: public Expr {
        Token variable;
        explicit VarExpr(Token variable): variable(std::move(variable)) {}
        Literal accept(ASTVisitor& visitor) override;
        ASTNodeType nodeType() override {return VarExprNode;}
    };

    struct CallExpr: public Expr {
        Token callee;
        std::vector<ExprPtr> args;
        CallExpr(Token callee, std::vector<ExprPtr> args)
        :callee(std::move(callee)), args(std::move(args)) {}
        Literal accept(ASTVisitor& visitor) override;
        ASTNodeType nodeType() override {return CallExprNode;}
    };

    struct UnaryExpr: public Expr {
        Token op;
        ExprPtr rhs;
        UnaryExpr(Token op, ExprPtr rhs)
        : op(std::move(op)), rhs(rhs) {}
        Literal accept(ASTVisitor& visitor) override;
        ASTNodeType nodeType() override {return UnaryExprNode;}
    };

    struct BinaryExpr: public Expr {
        Token op;
        ExprPtr lhs;
        ExprPtr rhs;
        BinaryExpr(Token op, ExprPtr lhs, ExprPtr rhs)
        : op(std::move(op)), lhs(lhs), rhs(rhs) {}
        Literal accept(ASTVisitor& visitor) override;
        ASTNodeType nodeType() override {return BinaryExprNode;}
    };

    struct GroupExpr: public Expr {
        ExprPtr expr;
        explicit GroupExpr(ExprPtr expr): expr(expr) {}
        Literal accept(ASTVisitor& visitor) override;
        ASTNodeType nodeType() override {return GroupExprNode;}
    };

    struct FuncDeclStmt: public Stmt {
        TypeTokenPair function;
        std::vector<TypeTokenPair> params;
        std::vector<StmtPtr> body;
        FuncDeclStmt(TypeTokenPair function, std::vector<TypeTokenPair> params, std::vector<StmtPtr> body)
        : function(std::move(function)), params(std::move(params)), body(std::move(body)) {}
        Literal accept(ASTVisitor& visitor) override;
        ASTNodeType nodeType() override {return FuncDeclStmtNode;}
    };

    struct VarDeclStmt: public Stmt {
        TypeTokenPair variable;
        ExprPtr expr;
        VarDeclStmt(TypeTokenPair variable, ExprPtr expr)
        : variable(std::move(variable)), expr(expr) {}
        Literal accept(ASTVisitor& visitor) override;
        ASTNodeType nodeType() override {return VarDeclStmtNode;}
    };

    struct AssignStmt: public Stmt {
        Token variable;
        ExprPtr expr;
        AssignStmt(Token variable, ExprPtr expr)
        : variable(std::move(variable)), expr(expr) {}
        Literal accept(ASTVisitor& visitor) override;
        ASTNodeType nodeType() override {return AssignStmtNode;}
    };

    struct IfStmt: public Stmt {
        ExprPtr condition;
        StmtPtr thenBranch;
        StmtPtr elseBranch;
        IfStmt(ExprPtr condition, StmtPtr thenBranch,
               StmtPtr elseBranch)
        : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}
        Literal accept(ASTVisitor& visitor) override;
        ASTNodeType nodeType() override {return IfStmtNode;}
    };

    struct WhileStmt: public Stmt {
        ExprPtr condition;
        StmtPtr body;
        WhileStmt(ExprPtr condition, StmtPtr body)
        : condition(condition), body(body) {}
        Literal accept(ASTVisitor& visitor) override;
        ASTNodeType nodeType() override {return WhileStmtNode;}
    };

    struct BlockStmt: public Stmt {
        std::vector<StmtPtr> statements;
        explicit BlockStmt(std::vector<StmtPtr> statements): statements(std::move(statements)) {}
        Literal accept(ASTVisitor& visitor) override;
        ASTNodeType nodeType() override {return BlockStmtNode;}
    };

    struct ReturnStmt: public Stmt {
        ExprPtr expr;
        explicit ReturnStmt(ExprPtr expr): expr(expr) {}
        Literal accept(ASTVisitor& visitor) override;
        ASTNodeType nodeType() override {return ReturnStmtNode;}
    };

    struct PrintStmt: public Stmt {
        ExprPtr expr;
        explicit PrintStmt(ExprPtr expr): expr(expr) {}
        Literal accept(ASTVisitor& visitor) override;
        ASTNodeType nodeType() override {return PrintStmtNode;}
    };
}
#endif //TINYC_AST_HPP
