#ifndef TINYCCOMPILER_STMT_HPP
#define TINYCCOMPILER_STMT_HPP
#include "AST.hpp"
#include <string>
#include <vector>
namespace tinyc {
enum StmtNodeType {
  UNDEFINED_STMT,
  DECL_STMT,
  EXPR_STMT,
  COMPOUND_STMT,
  IF_STMT,
  WHILE_STMT,
  RETURN_STMT,
  PRINT_STMT
};

class Stmt: public ASTNode {
protected:
public:
  void accept(ASTVisitor &visitor) override;
  [[nodiscard]] virtual StmtNodeType getNodeType() const {return UNDEFINED_STMT;}
};

class DeclStmt: public Stmt {
private:
  bool isArray = false;
  VarDecl* varDecl = nullptr;
  ArrayDecl* arrayDecl = nullptr;
public:
  DeclStmt(VarDecl* varDecl, ArrayDecl* arrayDecl);
  [[nodiscard]] StmtNodeType getNodeType() const override {return DECL_STMT;}
  [[nodiscard]] bool isArrayDecl() const {return isArray;}
  [[nodiscard]] VarDecl* getVarDecl() const {return varDecl;}
  [[nodiscard]] ArrayDecl* getArrayDecl() const {return arrayDecl;}
};

class ExprStmt: public Stmt {
private:
  ExprPtr expr;
public:
  explicit ExprStmt(ExprPtr expr);
  [[nodiscard]] StmtNodeType getNodeType() const override {return EXPR_STMT;}
  [[nodiscard]] ExprPtr getExpr() const {return expr;}
};

class CompoundStmt final: public Stmt {
private:
  std::vector<StmtPtr> body;
public:
  explicit CompoundStmt(std::vector<StmtPtr> body);
  [[nodiscard]] StmtNodeType getNodeType() const override {return COMPOUND_STMT;}
  [[nodiscard]] std::vector<StmtPtr> getBody() const {return body;}
};

class IfStmt final: public Stmt {
private:
  ExprPtr condition;
  CompoundStmt* thenBranch;
  CompoundStmt* elseBranch;
public:
  explicit IfStmt(ExprPtr condition, CompoundStmt* thenBranch, CompoundStmt* elseBranch);
  [[nodiscard]] StmtNodeType getNodeType() const override {return IF_STMT;}
  [[nodiscard]] ExprPtr getCondition() const {return condition;}
  [[nodiscard]] CompoundStmt* getThenBranch() const {return thenBranch;}
  [[nodiscard]] CompoundStmt* getElseBranch() const {return elseBranch;}
};

class WhileStmt final: public Stmt {
private:
  ExprPtr condition;
  CompoundStmt* loopBody;
public:
  explicit WhileStmt(ExprPtr condition, CompoundStmt* loopBody);
  [[nodiscard]] StmtNodeType getNodeType() const override {return WHILE_STMT;}
  [[nodiscard]] ExprPtr getCondition() const {return condition;}
  [[nodiscard]] CompoundStmt* getLoopBody() const {return loopBody;}
};

class ReturnStmt final: public Stmt {
private:
  ExprPtr expr;
public:
  explicit ReturnStmt(ExprPtr expr);
  [[nodiscard]] StmtNodeType getNodeType() const override {return RETURN_STMT;}
  [[nodiscard]] ExprPtr getReturnExpr() const {return expr;}
};

class PrintStmt final : public Stmt {
private:
  ExprPtr expr;
public:
  explicit PrintStmt(ExprPtr expr);
  [[nodiscard]] StmtNodeType getNodeType() const override {return PRINT_STMT;}
  [[nodiscard]] ExprPtr getPrintExpr() const {return expr;}
};
}

#endif //TINYCCOMPILER_STMT_HPP
