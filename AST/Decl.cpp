#include "Decl.hpp"
namespace tinyc {
Decl::Decl(TokenInfo type, TokenInfo name)
  : type(std::move(type)), name(std::move(name)) {}
void Decl::accept(ASTVisitor &visitor) {visitor.visit(this);}

VarDecl::VarDecl(const TokenInfo &type, const TokenInfo &name, ExprPtr expr)
  : Decl(type, name), expr(expr) {}

ArrayDecl::ArrayDecl(const TokenInfo &type, const TokenInfo &name, ExprPtr size, ListExpr* list)
  : Decl(type, name), list(list), size(size) {}

ParaDecl::ParaDecl(const TokenInfo &type, const TokenInfo &name, bool isRef)
  : Decl(type, name), isRef(isRef) {}

FuncDecl::FuncDecl(const TokenInfo &type, const TokenInfo &name,
                   const std::vector<ParaDecl *> &params, CompoundStmt *body)
  : Decl(type, name), params(params), body(body) {}

StructDecl::StructDecl(const TokenInfo &type, const TokenInfo &name, const std::vector<ParaDecl *> &members)
  : Decl(type, name), members(members) {}
}