#ifndef TINYCCOMPILER_DUMPER_HPP
#define TINYCCOMPILER_DUMPER_HPP
#include "AST.hpp"
#include <vector>
namespace tinyc {
class Dumper final : public ASTVisitor {
private:
  int indent = 0;
  int step = 2;
  std::vector<DeclPtr> ast;
  void increase();
  void decrease();

  static const char indicator = '-';
  static const std::string colorEnd;
  static const std::string positionBegin;
  static const std::string positionEnd;
  static const std::string declColorBegin;
  static const std::string stmtColorBegin;
  static const std::string exprColorBegin;
  static const std::string typeBegin;
  static const std::string typeEnd;
public:
  explicit Dumper(const std::vector<DeclPtr> &decls);
  void dump();
  void visit(Decl* node) override ;
  void visit(VarDecl* node) override ;
  void visit(ArrayDecl* node) override ;
  void visit(ParaDecl* node) override ;
  void visit(FuncDecl* node) override ;
  void visit(StructDecl* node) override ;

  void visit(Stmt* node) override ;
  void visit(DeclStmt* node) override ;
  void visit(ExprStmt* node) override ;
  void visit(CompoundStmt* node) override ;
  void visit(IfStmt* node) override ;
  void visit(WhileStmt* node) override ;
  void visit(ReturnStmt* node) override ;
  void visit(PrintStmt* node) override ;

  void visit(Expr* node) override ;
  void visit(AssignExpr* node) override ;
  void visit(VarExpr* node) override ;
  void visit(LiteralExpr* node) override ;
  void visit(ListExpr* node) override ;
  void visit(StructExpr* node) override;
  void visit(FuncCallExpr* node) override ;
  void visit(ArraySubscriptExpr* node) override;
  void visit(MemberAccessExpr* node) override ;
  void visit(UnaryExpr* node) override ;
  void visit(BinaryExpr* node) override ;
  void visit(GroupExpr* node) override ;
};
}
#endif //TINYCCOMPILER_DUMPER_HPP
