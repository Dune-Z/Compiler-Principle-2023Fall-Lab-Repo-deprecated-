#include "AST.hpp"
namespace tinyc {
    Literal LiteralExpr::accept(ASTVisitor& visitor) {return visitor.visit(this); }
    Literal VarExpr::accept(ASTVisitor& visitor) {return visitor.visit(this); }
    Literal CallExpr::accept(ASTVisitor& visitor) {return visitor.visit(this); }
    Literal UnaryExpr::accept(ASTVisitor& visitor) {return visitor.visit(this); }
    Literal BinaryExpr::accept(ASTVisitor& visitor) {return visitor.visit(this); }
    Literal GroupExpr::accept(ASTVisitor& visitor) {return visitor.visit(this); }

    Literal FuncDeclStmt::accept(ASTVisitor& visitor) {return visitor.visit(this); }
    Literal VarDeclStmt::accept(ASTVisitor& visitor) {return visitor.visit(this); }
    Literal AssignStmt::accept(ASTVisitor& visitor) {return visitor.visit(this); }
    Literal IfStmt::accept(ASTVisitor& visitor) {return visitor.visit(this); }
    Literal WhileStmt::accept(ASTVisitor& visitor) {return visitor.visit(this); }
    Literal ReturnStmt::accept(ASTVisitor& visitor) {return visitor.visit(this); }
    Literal PrintStmt::accept(ASTVisitor& visitor) {return visitor.visit(this); }
    Literal BlockStmt::accept(ASTVisitor& visitor) {return visitor.visit(this); }
}