#include "Tokens.hpp"
namespace tinyc {
TokenInfo::TokenInfo(int line, TokenKind kind, std::string lexeme)
  : line(line), kind(kind), lexeme(std::move(lexeme)) {}

std::ostream& operator<<(std::ostream &out, const TokenInfo &tokenInfo) {
  out << "line  : " << tokenInfo.line << std::endl;
  out << "kind  : " << tokenInfo.kind << std::endl;
  out << "lexeme: " << tokenInfo.lexeme << std::endl;
  return out;
}
}
