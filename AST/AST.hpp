#ifndef TINYCCOMPILER_AST_HPP
#define TINYCCOMPILER_AST_HPP
namespace tinyc {
// base class of AST node.
class ASTNode;
class ASTVisitor;
// classes of declaration.
class Decl;
class VarDecl;
class ArrayDecl;
class ParaDecl;
class FuncDecl;
class StructDecl;
// classes of statement.
class Stmt;
class DeclStmt;
class ExprStmt;
class CompoundStmt;
class IfStmt;
class WhileStmt;
class ReturnStmt;
class PrintStmt;
// classes of expression.
class Expr;
class AssignExpr;
class VarExpr;
class LiteralExpr;
class ListExpr;
class StructExpr;
class FuncCallExpr;
class ArraySubscriptExpr;
class MemberAccessExpr;
class UnaryExpr;
class BinaryExpr;
class GroupExpr;

using DeclPtr = Decl*;
using StmtPtr = Stmt*;
using ExprPtr = Expr*;

class ASTVisitor {
private:
public:
  ~ASTVisitor() = default;
  virtual void visit(Decl* node) = 0;
  virtual void visit(VarDecl* node) = 0;
  virtual void visit(ArrayDecl* node) = 0;
  virtual void visit(ParaDecl* node) = 0;
  virtual void visit(FuncDecl* node) = 0;
  virtual void visit(StructDecl* node) = 0;

  virtual void visit(Stmt* node) = 0;
  virtual void visit(DeclStmt* node) = 0;
  virtual void visit(ExprStmt* node) = 0;
  virtual void visit(CompoundStmt* node) = 0;
  virtual void visit(IfStmt* node) = 0;
  virtual void visit(WhileStmt* node) = 0;
  virtual void visit(ReturnStmt* node) = 0;
  virtual void visit(PrintStmt* node) = 0;

  virtual void visit(Expr* node) = 0;
  virtual void visit(AssignExpr* node) = 0;
  virtual void visit(VarExpr* node) = 0;
  virtual void visit(LiteralExpr* node) = 0;
  virtual void visit(ListExpr* node) = 0;
  virtual void visit(StructExpr* node) = 0;
  virtual void visit(FuncCallExpr* node) = 0;
  virtual void visit(ArraySubscriptExpr* node) = 0;
  virtual void visit(MemberAccessExpr* node) = 0;
  virtual void visit(UnaryExpr* node) = 0;
  virtual void visit(BinaryExpr* node) = 0;
  virtual void visit(GroupExpr* node) = 0;
};

class ASTNode {
private:
public:
  ASTNode() = default;
  virtual ~ASTNode()  =default;
  virtual void accept(ASTVisitor &visitor) = 0;
};
}

#endif //TINYCCOMPILER_AST_HPP
