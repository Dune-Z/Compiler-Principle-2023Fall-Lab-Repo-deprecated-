#include "Parser.hpp"
#include "AST/Decl.hpp"
namespace tinyc {
DeclPtr Parser::parseDecl() {
  if(match(FN)) return parseFuncDecl();
  else if(match(LET)) return parseVarDecl();
  else if(match(STRUCT)) return parseStructDecl();
  throw std::runtime_error("Only declarations are allowed in global scope.");
}

FuncDecl* Parser::parseFuncDecl() {
  auto nameToken= consume(IDENTIFIER, "Expect function name.");
  consume(LEFT_PAREN, "Expect '('.");
  std::vector<ParaDecl*> params;
  while(!match(RIGHT_PAREN)) {
    params.push_back(parseParaDecl());
    if(match(RIGHT_PAREN)) break;
    consume(COMMA, "Expect ','.");
  }
  consume(ARROW, "Expect '->' to annotate return type.");
  // Function has additional type: void.
  if(!match(TYPE_INT, TYPE_BOOLEAN, TYPE_FLOAT, TYPE_STRING, TYPE_VOID)) {
    throw std::runtime_error("Expect type annotation.");
  }
  auto typeToken = previous();
  consume(LEFT_BRACE, "Expect '{'.");
  auto body = parseCompoundStmt();
  return new FuncDecl(typeToken, nameToken, params, body);
}

ParaDecl* Parser::parseParaDecl() {
  auto nameToken = consume(IDENTIFIER, "Expect argument name.");
  consume(COLON, "Expect ':'.");
  bool isRef = false;
  if(match(REF)) isRef = true;
  if(!match(TYPE_INT, TYPE_BOOLEAN, TYPE_FLOAT, TYPE_STRING, IDENTIFIER)) {
    throw std::runtime_error("Expect type annotation.");
  }
  auto typeToken = previous();
  return new ParaDecl(typeToken, nameToken, isRef);
}

DeclPtr Parser::parseVarDecl() {
  auto nameToken = consume(IDENTIFIER, "Expect variable name.");
  if(match(LEFT_BRACKET)) {
    auto size = parseExpr();
    consume(RIGHT_BRACKET, "Expect ']'.");
    consume(COLON, "Expect ':' to annotate variable type.");
    if(!match(TYPE_INT, TYPE_BOOLEAN, TYPE_FLOAT, TYPE_STRING)) {
      throw std::runtime_error("Expect type annotation.");
    }
    auto typeToken = previous();
    ListExpr* list = nullptr;
    if(match(OPERATOR_EQUAL)) {
      consume(LEFT_BRACE, "Expect '{'.");
      list = parseListExpr();
    }
    consume(SEMICOLON, "Expect ';'");
    return new ArrayDecl(typeToken, nameToken, size, list);
  }
  // Not array variable.
  consume(COLON, "Expect ':' to annotate variable type.");
  if(!match(TYPE_INT, TYPE_BOOLEAN, TYPE_FLOAT, TYPE_STRING, IDENTIFIER)) {
    throw std::runtime_error("Expect type annotation.");
  }
  auto typeToken = previous();
  ExprPtr expr = nullptr;
  if(match(OPERATOR_EQUAL)) expr = parseExpr();
  consume(SEMICOLON, "Expect ';'");
  return new VarDecl(typeToken, nameToken, expr);
}

DeclPtr Parser::parseStructDecl() {
  auto structure = previous();
  auto nameToken = consume(IDENTIFIER, "Expect struct name.");
  consume(LEFT_BRACE, "Expect '{'.");
  std::vector<ParaDecl*> members;
  while(!match(RIGHT_BRACE)) {
    members.push_back(parseParaDecl());
    if(match(RIGHT_BRACE)) break;
    consume(COMMA, "Expect ','.");
  }
//  consume(SEMICOLON, "Expect ';'");
  return new StructDecl(structure, nameToken, members);
}
}