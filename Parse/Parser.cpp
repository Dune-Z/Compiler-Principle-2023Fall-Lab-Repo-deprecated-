#include "Parser.hpp"
#include "Lex/Lexer.hpp"
#include <fstream>
#include <sstream>
namespace tinyc {
Parser::Parser(const std::string &filename) {
  std::ifstream file(filename);
  std::stringstream buffer;
  buffer << file.rdbuf();
  const std::string source(buffer.str());
  Lexer lexer(source);
  tokens = lexer.scanTokens();
  current = tokens.begin();
}

std::vector<DeclPtr> Parser::parse() {
  std::vector<DeclPtr> decls;
  while(!isAtEnd()) decls.push_back(parseDecl());
  current = tokens.begin();
  return decls;
}

bool Parser::isAtEnd() const { return current->getKind() == FEOF; }
bool Parser::check(TokenKind kind) const { return lookahead().getKind() == kind; }
TokenInfo Parser::consume(TokenKind kind, std::string &&message) {
  if(check(kind)) return advance();
  throw std::runtime_error(message);
}
TokenInfo Parser::advance() {
  if(!isAtEnd()) current++;
  return previous();
}
TokenInfo Parser::previous() const { return *(current - 1); }
TokenInfo Parser::lookahead() const { return *current; }
}