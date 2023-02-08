#include "Stmt.hpp"
namespace tinyc {
void Stmt::accept(ASTVisitor &visitor) {visitor.visit(this);}

CompoundStmt::CompoundStmt(std::vector<StmtPtr> body)
  : body(std::move(body)) {}

DeclStmt::DeclStmt(VarDecl *varDecl, ArrayDecl *arrayDecl)
  : varDecl(varDecl), arrayDecl(arrayDecl) {isArray = (arrayDecl != nullptr);}

ExprStmt::ExprStmt(ExprPtr expr)
  : expr(expr) {}

IfStmt::IfStmt(ExprPtr condition, CompoundStmt* thenBranch, CompoundStmt* elseBranch)
  : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}

WhileStmt::WhileStmt(ExprPtr condition, CompoundStmt* loopBody)
  : condition(condition), loopBody(loopBody) {}

ReturnStmt::ReturnStmt(ExprPtr expr)
  : expr(expr) {}

PrintStmt::PrintStmt(ExprPtr expr)
  : expr(expr) {}
}