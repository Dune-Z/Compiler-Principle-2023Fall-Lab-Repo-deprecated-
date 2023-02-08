#include "Dumper.hpp"
#include "Decl.hpp"
#include "Stmt.hpp"
#include "Expr.hpp"
#include <iostream>
namespace tinyc {
Dumper::Dumper(const std::vector<DeclPtr> &ast): ast(ast) {}
const std::string Dumper::colorEnd = "\033[0m";
const std::string Dumper::positionBegin = "\033[1;33m <line: ";
const std::string Dumper::positionEnd = ">\033[0m ";
const std::string Dumper::declColorBegin= "\033[1;35m";
const std::string Dumper::stmtColorBegin= "\033[1;32m";
const std::string Dumper::exprColorBegin= "\033[1;34m";
const std::string Dumper::typeBegin = " \033[32m'";
const std::string Dumper::typeEnd = "'\033[0m";

void Dumper::increase() { indent += step; }
void Dumper::decrease() { indent -= step; }

void Dumper::dump() {
  for(auto &decl: ast) visit(decl);
}

void Dumper::visit(Decl* node) {
  switch(node->getNodeType()) {
    case VAR_DECL: visit(dynamic_cast<VarDecl*>(node)); break;
    case ARRAY_DECL: visit(dynamic_cast<ArrayDecl*>(node)); break;
    case FUNC_DECL: visit(dynamic_cast<FuncDecl*>(node)); break;
    case PARAMS_DECL: visit(dynamic_cast<ParaDecl*>(node)); break;
    case STRUCT_DECL: visit(dynamic_cast<StructDecl*>(node)); break;
    case UNDEFINED_DECL:
    default:
      throw std::runtime_error("Undefined Decl node.");
  }
}

void Dumper::visit(VarDecl* node) {
  std::cout << std::string(indent, indicator);
  std::cout << declColorBegin << "VarDecl" << colorEnd;
  std::cout << positionBegin << node->getLocation() << positionEnd;
  std::cout << node->getName() << typeBegin << node->getTypeName() << typeEnd << std::endl;
  increase();
  visit(node->getExpr());
  decrease();
}
void Dumper::visit(ArrayDecl* node) {
  std::cout << std::string(indent, indicator);
  std::cout << declColorBegin << "ArrayDecl" << colorEnd;
  std::cout << positionBegin << node->getLocation() << positionEnd;
  std::cout << node->getName() << typeBegin << node->getTypeName() << typeEnd << std::endl;
  increase();
  visit(node->getSize());
  visit(node->getExpr());
  decrease();
}
void Dumper::visit(ParaDecl* node) {
  std::cout << std::string(indent, indicator);
  std::cout << declColorBegin << "ParaDecl" << colorEnd;
  std::cout << positionBegin << node->getLocation() << positionEnd;
  std::cout << node->getName() << typeBegin << node->getTypeName();
  if(node->getIsRef()) std::cout << " Reference";
  std::cout << typeEnd << std::endl;
}
void Dumper::visit(FuncDecl* node) {
  std::cout << std::string(indent, indicator);
  std::cout << declColorBegin << "FuncDecl" << colorEnd;
  std::cout << positionBegin << node->getLocation() << positionEnd;
  std::cout << node->getName() << typeBegin << node->getTypeName() << typeEnd << std::endl;
  increase();
  for(const auto &x: node->getParams()) visit(x);
  visit(node->getBody());
  decrease();
}
void Dumper::visit(StructDecl* node) {
  std::cout << std::string(indent, indicator);
  std::cout << declColorBegin << "StructDecl" << colorEnd;
  std::cout << positionBegin << node->getLocation() << positionEnd;
  std::cout << node->getName() << std::endl;
  increase();
  for(const auto &x: node->getMembers()) visit(x);
  decrease();
}

void Dumper::visit(Stmt* node) {
  switch (node->getNodeType()) {
    case DECL_STMT: visit(dynamic_cast<DeclStmt*>(node)); break;
    case EXPR_STMT: visit(dynamic_cast<ExprStmt*>(node)); break;
    case COMPOUND_STMT: visit(dynamic_cast<CompoundStmt*>(node)); break;
    case IF_STMT: visit(dynamic_cast<IfStmt*>(node)); break;
    case WHILE_STMT: visit(dynamic_cast<WhileStmt*>(node)); break;
    case RETURN_STMT: visit(dynamic_cast<ReturnStmt*>(node)); break;
    case PRINT_STMT: visit(dynamic_cast<PrintStmt*>(node)); break;
    case UNDEFINED_STMT:
    default:
      throw std::runtime_error("Undefined Stmt node.");
  }
}
void Dumper::visit(DeclStmt* node) {
  std::cout << std::string(indent, indicator);
  std::cout << stmtColorBegin << "DeclStmt" << colorEnd << std::endl;
  if(node->isArrayDecl()) {
    auto decl = node->getArrayDecl();
    increase();
    visit(decl);
    decrease();
  } else {
    auto decl = node->getVarDecl();
    increase();
    visit(decl);
    decrease();
  }
}
void Dumper::visit(ExprStmt* node) {
  std::cout << std::string(indent, indicator);
  std::cout << stmtColorBegin << "ExprStmt" << colorEnd << std::endl;
  auto expr = node->getExpr();
  increase();
  visit(expr);
  decrease();
}
void Dumper::visit(CompoundStmt* node) {
  std::cout << std::string(indent, indicator);
  std::cout << stmtColorBegin << "CompoundStmt" << colorEnd << std::endl;
  increase();
  for(const auto &x: node->getBody()) visit(x);
  decrease();
}
void Dumper::visit(IfStmt* node) {
  std::cout << std::string(indent, indicator);
  std::cout << stmtColorBegin << "IfStmt" << colorEnd << std::endl;
  increase();
  visit(node->getCondition());
  visit(node->getThenBranch());
  if(node->getElseBranch() != nullptr){
    visit(node->getElseBranch());
  }
  decrease();
}
void Dumper::visit(WhileStmt* node) {
  std::cout << std::string(indent, indicator);
  std::cout << stmtColorBegin << "WhileStmt" << colorEnd << std::endl;
  increase();
  visit(node->getCondition());
  visit(node->getLoopBody());
  decrease();
}
void Dumper::visit(ReturnStmt* node) {
  std::cout << std::string(indent, indicator);
  std::cout << stmtColorBegin << "ReturnStmt" << colorEnd << std::endl;
  auto expr = node->getReturnExpr();
  if(expr != nullptr) {
    increase();
    visit(expr);
    decrease();
  }
}
void Dumper::visit(PrintStmt* node) {
  std::cout << std::string(indent, indicator);
  std::cout << stmtColorBegin << "PrintStmt" << colorEnd << std::endl;
  auto expr = node->getPrintExpr();
  increase();
  visit(expr);
  decrease();
}

void Dumper::visit(Expr* node) {
  switch (node->getNodeType()) {
    case ASSIGN_EXPR: visit(dynamic_cast<AssignExpr *>(node)); break;
    case VAR_EXPR: visit(dynamic_cast<VarExpr *>(node)); break;
    case ARRAY_SUBSCRIPT_EXPR: visit(dynamic_cast<ArraySubscriptExpr *>(node)); break;
    case MEMBER_ACCESS_EXPR: visit(dynamic_cast<MemberAccessExpr *>(node)); break;
    case LITERAL_EXPR: visit(dynamic_cast<LiteralExpr *>(node)); break;
    case LIST_EXPR: visit(dynamic_cast<ListExpr *>(node)); break;
    case STRUCT_EXPR: visit(dynamic_cast<StructExpr *>(node)); break;
    case FUNC_CALL_EXPR: visit(dynamic_cast<FuncCallExpr *>(node)); break;
    case UNARY_EXPR: visit(dynamic_cast<UnaryExpr *>(node)); break;
    case BINARY_EXPR: visit(dynamic_cast<BinaryExpr *>(node)); break;
    case GROUP_EXPR: visit(dynamic_cast<GroupExpr *>(node)); break;
    case UNDEFINED_EXPR:
    default:
      throw std::runtime_error("Undefined Expr node.");
  }
}
void Dumper::visit(AssignExpr* node) {
  std::cout << std::string(indent, indicator);
  std::cout << exprColorBegin << "AssignExpr" << colorEnd << std::endl;
  increase();
  auto name = node->getName();
  if(name->getNodeType() == VAR_EXPR) {
    visit(dynamic_cast<VarExpr*>(name));
  } else if(name->getNodeType() == MEMBER_ACCESS_EXPR) {
    visit(dynamic_cast<MemberAccessExpr*>(name));
  }
  visit(node->getExpr());
  decrease();
}
void Dumper::visit(VarExpr* node) {
  std::cout << std::string(indent, indicator);
  std::cout << exprColorBegin << "VarExpr" << colorEnd;
  auto var = node->getVarName();
  std::cout << positionBegin << var.getLocation() << positionEnd;
  std::cout << var.getLexeme() << std::endl;
}
void Dumper::visit(LiteralExpr* node) {
  std::cout << std::string(indent, indicator);
  std::cout << exprColorBegin << "LiteralExpr" << colorEnd;
  auto literal = node->getLiteral();
  std::cout << positionBegin << literal.getLocation() << positionEnd;
  std::cout << literal.getLexeme() << typeBegin;
  if(literal.getKind() == NUMBER) std::cout << "Number";
  else std::cout << "String";
  std::cout << typeEnd << std::endl;
}
void Dumper::visit(ListExpr* node) {
  std::cout << std::string(indent, indicator);
  std::cout << exprColorBegin << "ListExpr" << colorEnd << std::endl;
  increase();
  for(const auto &x: node->getListMember()) visit(x);
  decrease();
}
void Dumper::visit(StructExpr* node) {
  std::cout << std::string(indent, indicator);
  std::cout << exprColorBegin << "StructExpr" << colorEnd;
  auto var = node->getTypeName();
  std::cout << positionBegin << var.getLocation() << positionEnd;
  std::cout << var.getLexeme() << std::endl;
  increase();
  visit(node->getMembers());
  decrease();
}
void Dumper::visit(FuncCallExpr* node) {
  std::cout << std::string(indent, indicator);
  std::cout << exprColorBegin << "FuncCallExpr" << colorEnd;
  auto callee = node->getCallee();
  auto args = node->getArgs();
  std::cout << positionBegin << callee.getLocation() << positionEnd;
  std::cout << callee.getLexeme() << std::endl;
  increase();
  visit(args);
  decrease();
}
void Dumper::visit(ArraySubscriptExpr* node) {
  std::cout << std::string(indent, indicator);
  std::cout << exprColorBegin << "ArraySubscriptExpr" << colorEnd;
  auto var = node->getVarName();
  std::cout << positionBegin << var.getLocation() << positionEnd;
  std::cout << var.getLexeme() << std::endl;
  increase();
  visit(node->getIndex());
  decrease();
}
void Dumper::visit(MemberAccessExpr* node) {
  std::cout << std::string(indent, indicator);
  std::cout << exprColorBegin << "MemberAccessExpr" << colorEnd;
  auto var = node->getVarName();
  std::cout << positionBegin << var.getLocation() << positionEnd;
  std::cout << var.getLexeme() << std::endl;
  increase();
  visit(node->getMember());
  decrease();
}
void Dumper::visit(UnaryExpr* node) {
  std::cout << std::string(indent, indicator);
  std::cout << exprColorBegin << "UnaryExpr" << colorEnd;
  auto op = node->getOp();
  std::cout << positionBegin << op.getLocation() << positionEnd;
  std::cout << op.getLexeme() << std::endl;
  increase();
  visit(node->getRhsExpr());
  decrease();
}
void Dumper::visit(BinaryExpr* node) {
  std::cout << std::string(indent, indicator);
  std::cout << exprColorBegin << "BinaryExpr" << colorEnd;
  auto op = node->getOp();
  std::cout << positionBegin << op.getLocation() << positionEnd;
  std::cout << op.getLexeme() << std::endl;
  increase();
  visit(node->getLhsExpr());
  visit(node->getRhsExpr());
  decrease();
}
void Dumper::visit(GroupExpr* node) {
  std::cout << std::string(indent, indicator);
  std::cout << exprColorBegin << "GroupExpr" << colorEnd << std::endl;
  increase();
  visit(node->getExpr());
  decrease();
}
}