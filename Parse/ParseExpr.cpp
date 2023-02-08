#include "Parser.hpp"
#include "AST/Expr.hpp"
namespace tinyc {
ExprPtr Parser::parseExpr() {
  if(match(LEFT_BRACE)) {
    return parseListExpr();
  }
  return parseAssignExpr();
}

ExprPtr Parser::parseAssignExpr() {
  auto expr = parseLogicOrExpr();
  if(match(OPERATOR_EQUAL)) {
    auto value = parseExpr();
    auto varExpr = dynamic_cast<VarExpr*>(expr);
    if(varExpr != nullptr)
      return new AssignExpr(varExpr, value);
    throw std::runtime_error("Invalid identifier to be assigned.");
  }
  return expr;
}

ListExpr* Parser::parseListExpr() {
  std::vector<ExprPtr> list;
  while(!match(RIGHT_BRACE)) {
    auto expr = parseExpr();
    list.push_back(expr);
    if(match(RIGHT_BRACE)) break;
    consume(COMMA, "Expect ','.");
  }
  return new ListExpr(list);
}

ListExpr* Parser::parseArgsExpr() {
  std::vector<ExprPtr> list;
  while(!match(RIGHT_PAREN)) {
    auto expr = parseExpr();
    list.push_back(expr);
    if(match(RIGHT_PAREN)) break;
    consume(COMMA, "Expect ','.");
  }
  return new ListExpr(list);
}

ExprPtr Parser::parseLogicOrExpr() {
  ExprPtr expr = parseLogicAndExpr();
  if(match(OPERATOR_OR)) {
    auto op = previous();
    auto rest = parseLogicOrExpr();
    expr = new BinaryExpr(op, expr, rest);
  }
  return expr;
}

ExprPtr Parser::parseLogicAndExpr() {
  ExprPtr expr = parseEqualityExpr();
  if(match(OPERATOR_AND)) {
    auto op = previous();
    auto rest = parseLogicAndExpr();
    expr = new BinaryExpr(op, expr, rest);
  }
  return expr;
}

ExprPtr Parser::parseEqualityExpr() {
  ExprPtr expr = parseComparisonExpr();
  if(match(OPERATOR_BANG_EQUAL, OPERATOR_EQUAL_EQUAL)) {
    auto op = previous();
    auto rest = parseEqualityExpr();
    expr = new BinaryExpr(op, expr, rest);
  }
  return expr;
}

ExprPtr Parser::parseComparisonExpr() {
  ExprPtr expr = parseTermExpr();
  if(match(OPERATOR_LESS, OPERATOR_LESS_EQUAL, OPERATOR_GREATER, OPERATOR_GREATER_EQUAL)) {
    auto op = previous();
    auto rest = parseComparisonExpr();
    expr = new BinaryExpr(op, expr, rest);
  }
  return expr;
}

ExprPtr Parser::parseTermExpr() {
  ExprPtr expr = parseFactorExpr();
  if(match(OPERATOR_ADD, OPERATOR_SUB)) {
    auto op = previous();
    auto rest = parseTermExpr();
    expr = new BinaryExpr(op, expr, rest);
  }
  return expr;
}

ExprPtr Parser::parseFactorExpr() {
  ExprPtr expr = parseUnaryExpr();
  if(match(OPERATOR_MUL, OPERATOR_DIV)) {
    auto op = previous();
    auto rest = parseFactorExpr();
    expr = new BinaryExpr(op, expr, rest);
  }
  return expr;
}

ExprPtr Parser::parseUnaryExpr() {
  if(match(OPERATOR_BANG, OPERATOR_SUB)) {
    auto op = previous();
    auto expr = parseCallExpr();
    return new UnaryExpr(op, expr);
  } else return parseCallExpr();
}

ExprPtr Parser::parseCallExpr() {
  if(match(IDENTIFIER)) {
    auto callee = previous();
    if(match(DOT)) {
      auto member = parseCallExpr();
      return new MemberAccessExpr(callee, member);
    } else if(match(LEFT_PAREN)) {
      auto list = parseArgsExpr();
      return new FuncCallExpr(callee, list);
    } else if(match(LEFT_BRACKET)) {
      auto index = parseExpr();
      assert(index->getNodeType() != LIST_EXPR);
      consume(RIGHT_BRACKET, "Expect ']'.");
      return new ArraySubscriptExpr(callee, index);
    } else if(match(LEFT_BRACE)) {
      auto list = parseListExpr();
      return new StructExpr(callee, list);
    }
    return new VarExpr(callee);
  } else {
    return parsePrimaryExpr();
  }
}

ExprPtr Parser::parsePrimaryExpr() {
  if(match(NUMBER) || match(STRING)){
    TokenInfo literal = previous();
    return new LiteralExpr(literal);
  }
  if(match(LEFT_PAREN)) {
    auto expr = parseExpr();
    consume(RIGHT_PAREN, "Expect ')' after expression");
    return new GroupExpr(expr);
  }
  throw std::runtime_error("invalid primary expr.");
}
}