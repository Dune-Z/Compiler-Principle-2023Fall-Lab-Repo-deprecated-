#include "Expr.hpp"
namespace tinyc {
void Expr::accept(ASTVisitor &visitor) {visitor.visit(this);}

AssignExpr::AssignExpr(ExprPtr name, ExprPtr expr): name(name), expr(expr) {}

VarExpr::VarExpr(TokenInfo name): name(std::move(name)) {}

LiteralExpr::LiteralExpr(TokenInfo literal)
  : literal(std::move(literal)) {}

ListExpr::ListExpr(std::vector<ExprPtr> list): list(std::move(list)) {}

StructExpr::StructExpr(TokenInfo typeName, ListExpr *list)
  : typeName(std::move(typeName)), list(list) {}

ArraySubscriptExpr::ArraySubscriptExpr(TokenInfo arrayName, ExprPtr index)
  : VarExpr(std::move(arrayName)), index(index) {}

FuncCallExpr::FuncCallExpr(TokenInfo callee, ListExpr *args)
  : callee(std::move(callee)), args(args) {}

MemberAccessExpr::MemberAccessExpr(TokenInfo base, ExprPtr member)
  : VarExpr(std::move(base)), member(member) {}

UnaryExpr::UnaryExpr(TokenInfo op, ExprPtr rhs): op(std::move(op)), rhs(rhs) {}

BinaryExpr::BinaryExpr(TokenInfo op, ExprPtr lhs, ExprPtr rhs): op(std::move(op)), lhs(lhs), rhs(rhs) {}

GroupExpr::GroupExpr(ExprPtr expr): expr(expr) {}
}