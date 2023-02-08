#ifndef TINYCCOMPILER_EXPR_HPP
#define TINYCCOMPILER_EXPR_HPP
#include "AST.hpp"
#include "Lex/Tokens.hpp"
#include <string>
#include <vector>
namespace tinyc {
enum ExprNodeType {
  UNDEFINED_EXPR,
  ASSIGN_EXPR,
  VAR_EXPR,
  ARRAY_SUBSCRIPT_EXPR,
  MEMBER_ACCESS_EXPR,
  LITERAL_EXPR,
  LIST_EXPR,
  STRUCT_EXPR,
  FUNC_CALL_EXPR,
  UNARY_EXPR,
  BINARY_EXPR,
  GROUP_EXPR
};

class Expr: public ASTNode {
public:
  void accept(ASTVisitor &visitor) override;
  virtual ExprNodeType getNodeType() {return UNDEFINED_EXPR;}
};

class AssignExpr final : public Expr {
private:
  ExprPtr name;
  ExprPtr expr;
public:
  AssignExpr(ExprPtr name, ExprPtr expr);
  ExprNodeType getNodeType() override {return ASSIGN_EXPR;}
  [[nodiscard]] ExprPtr getName() const {return name;}
  [[nodiscard]] ExprPtr getExpr() const {return expr;}
};

class VarExpr: public Expr {
protected:
  TokenInfo name;
public:
  explicit VarExpr(TokenInfo name);
  ExprNodeType getNodeType() override {return VAR_EXPR;}
  [[nodiscard]] TokenInfo getVarName() const {return name;}
};

class ArraySubscriptExpr final : public VarExpr {
private:
  ExprPtr index;
public:
  ArraySubscriptExpr(TokenInfo arrayName, ExprPtr index);
  ExprNodeType getNodeType() override {return ARRAY_SUBSCRIPT_EXPR;}
  [[nodiscard]] ExprPtr getIndex() const {return index;}
};

class MemberAccessExpr final : public VarExpr {
private:
  ExprPtr member;
public:
  MemberAccessExpr(TokenInfo base, ExprPtr member);
  ExprNodeType getNodeType() override {return MEMBER_ACCESS_EXPR;}
  [[nodiscard]] ExprPtr getMember() const {return member;}
};

class LiteralExpr final : public Expr {
private:
  TokenInfo literal;
public:
  explicit LiteralExpr(TokenInfo literal);
  ExprNodeType getNodeType() override {return LITERAL_EXPR;}
  [[nodiscard]] TokenInfo getLiteral() const {return literal;}
};

class ListExpr final : public Expr {
private:
  std::vector<ExprPtr> list;
public:
  explicit ListExpr(std::vector<ExprPtr> list);
  ExprNodeType getNodeType() override {return LIST_EXPR;}
  [[nodiscard]] std::vector<ExprPtr> getListMember() const {return list;}
};

class StructExpr final : public Expr {
private:
  TokenInfo typeName;
  ListExpr* list;
public:
  StructExpr(TokenInfo typeName, ListExpr* list);
  ExprNodeType getNodeType() override {return STRUCT_EXPR;}
  [[nodiscard]] TokenInfo getTypeName() const {return typeName;}
  [[nodiscard]] ListExpr* getMembers() const {return list;}
};

class FuncCallExpr final : public Expr {
private:
  TokenInfo callee;
  ListExpr* args;
public:
  FuncCallExpr(TokenInfo callee, ListExpr* args);
  ExprNodeType getNodeType() override {return FUNC_CALL_EXPR;}
  [[nodiscard]] TokenInfo getCallee() const {return callee;}
  [[nodiscard]] ListExpr* getArgs() const {return args;}
};

class UnaryExpr final : public Expr {
private:
  TokenInfo op;
  ExprPtr rhs;
public:
  UnaryExpr(TokenInfo op, ExprPtr rhs);
  ExprNodeType getNodeType() override {return UNARY_EXPR;}
  [[nodiscard]] TokenInfo getOp() const {return op;}
  [[nodiscard]] ExprPtr getRhsExpr() const {return rhs;}
};

class BinaryExpr final : public Expr {
private:
  TokenInfo op;
  ExprPtr lhs;
  ExprPtr rhs;
public:
  BinaryExpr(TokenInfo op, ExprPtr lhs, ExprPtr rhs);
  ExprNodeType getNodeType() override {return BINARY_EXPR;}
  [[nodiscard]] TokenInfo getOp() const {return op;}
  [[nodiscard]] ExprPtr getLhsExpr() const {return lhs;}
  [[nodiscard]] ExprPtr getRhsExpr() const {return rhs;}
};

class GroupExpr final : public Expr {
private:
  ExprPtr expr;
public:
  explicit GroupExpr(ExprPtr expr);
  ExprNodeType getNodeType() override {return GROUP_EXPR;}
  [[nodiscard]] ExprPtr getExpr() const {return expr;}
};

}

#endif //TINYCCOMPILER_EXPR_HPP
