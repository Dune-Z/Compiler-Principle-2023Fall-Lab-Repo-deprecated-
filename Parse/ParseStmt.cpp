#include "Parser.hpp"
#include "AST/Decl.hpp"
#include "AST/Stmt.hpp"
namespace tinyc {
CompoundStmt* Parser::parseCompoundStmt() {
  std::vector<StmtPtr> body;
  while(!match(RIGHT_BRACE)) {
    body.push_back(parseStmt());
  }
  return new CompoundStmt(body);
}

StmtPtr Parser::parseStmt() {
  if(match(IF)) return parseIfStmt();
  else if(match(WHILE)) return parseWhileStmt();
  else if(match(RETURN)) return parseReturnStmt();
  else if(match(PRINT)) return parsePrintStmt();
  else if(match(LET)) return parseDeclStmt();
  else if(match(RIGHT_BRACE)) return parseCompoundStmt();
  return parseExprStmt();
}

DeclStmt* Parser::parseDeclStmt() {
  auto decl = parseVarDecl();
  return new DeclStmt(dynamic_cast<VarDecl*>(decl), dynamic_cast<ArrayDecl*>(decl));
}

ExprStmt* Parser::parseExprStmt() {
  auto expr = parseExpr();
  consume(SEMICOLON, "Expect ';'.");
  return new ExprStmt(expr);
}

IfStmt* Parser::parseIfStmt() {
  consume(LEFT_PAREN, "Expect '('.");
  auto condition = parseExpr();
  consume(RIGHT_PAREN, "Expect ')'.");
  consume(LEFT_BRACE, "Expect '{'");
  auto thenBranch = parseCompoundStmt();
  CompoundStmt* elseBranch = nullptr;
  if(match(ELSE)) {
    consume(LEFT_BRACE, "Expect '{'.");
    elseBranch = parseCompoundStmt();
  }
  return new IfStmt(condition, thenBranch, elseBranch);
}

WhileStmt* Parser::parseWhileStmt() {
  consume(LEFT_PAREN, "Expect '('.");
  auto condition = parseExpr();
  consume(RIGHT_PAREN, "Expect ')'.");
  consume(LEFT_BRACE, "Expect '{'");
  auto loopBody = parseCompoundStmt();
  return new WhileStmt(condition, loopBody);
}

ReturnStmt* Parser::parseReturnStmt() {
  ExprPtr expr = nullptr;
  if(!match(SEMICOLON)) {
    expr = parseExpr();
    consume(SEMICOLON, "Expect ';'.");
  }
  return new ReturnStmt(expr);
}

PrintStmt* Parser::parsePrintStmt() {
  consume(LEFT_PAREN, "Expect '('.");
  auto expr = parseExpr();
  consume(RIGHT_PAREN, "Expect ')'.");
  consume(SEMICOLON, "Expect ';'.");
  return new PrintStmt(expr);
}

}