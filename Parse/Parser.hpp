#ifndef TINYCCOMPILER_PARSER_HPP
#define TINYCCOMPILER_PARSER_HPP
#include "AST/AST.hpp"
#include "Lex/Tokens.hpp"
#include <string>
#include <vector>

namespace tinyc {
class Parser {
private:
  std::vector<TokenInfo> tokens;
  std::vector<TokenInfo>::const_iterator current;
  [[nodiscard]] bool isAtEnd() const;
  [[nodiscard]] bool check(TokenKind kind) const;
  TokenInfo consume(TokenKind kind, std::string&& message);

  TokenInfo advance();
  [[nodiscard]] TokenInfo previous() const;
  [[nodiscard]] TokenInfo lookahead() const;

  template<typename Head, typename... Tails>
  bool match(const Head& head, const Tails&... tails);
  template<typename Head>
  bool match(const Head& head);

  // Parse AST in recursive descend manner.
  // Grammar: See document.
  DeclPtr parseDecl();
  DeclPtr parseVarDecl();
  FuncDecl* parseFuncDecl();
  ParaDecl* parseParaDecl();
  DeclPtr parseStructDecl();

  StmtPtr parseStmt();
  CompoundStmt* parseCompoundStmt();
  DeclStmt* parseDeclStmt();
  ExprStmt* parseExprStmt();
  IfStmt* parseIfStmt();
  WhileStmt* parseWhileStmt();
  ReturnStmt* parseReturnStmt();
  PrintStmt* parsePrintStmt();

  ExprPtr parseExpr();
  ExprPtr parseAssignExpr();
  ListExpr* parseListExpr();
  ListExpr* parseArgsExpr();
  ExprPtr parseLogicOrExpr();
  ExprPtr parseLogicAndExpr();
  ExprPtr parseEqualityExpr();
  ExprPtr parseComparisonExpr();
  ExprPtr parseTermExpr();
  ExprPtr parseFactorExpr();
  ExprPtr parseUnaryExpr();
  ExprPtr parseCallExpr();
  ExprPtr parsePrimaryExpr();


public:
  explicit Parser(const std::string &filename);
  std::vector<DeclPtr> parse();
};

template <typename Head, typename... Tails>
bool Parser::match(const Head &head, const Tails &...tails) {
  if(check(head)){
    advance();
    return true;
  } else return match(tails...);
}

template <typename Head>
bool Parser::match(const Head &head) {
  if(check(head)){
    advance();
    return true;
  } else return false;
}
}

#endif //TINYCCOMPILER_PARSER_HPP
