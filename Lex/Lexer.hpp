#ifndef TINYCCOMPILER_LEXER_HPP
#define TINYCCOMPILER_LEXER_HPP
#include "Tokens.hpp"
#include <vector>
#include <unordered_map>
namespace tinyc {
class Lexer {
private:
  int currentLine = 1;
  std::string source;
  std::string::const_iterator start;
  std::string::const_iterator current;
  std::vector<TokenInfo> tokens;
  std::unordered_map<std::string, TokenKind> keywords = {
          {"int",    TYPE_INT},
          {"bool",   TYPE_BOOLEAN},
          {"float",  TYPE_FLOAT},
          {"str",    TYPE_STRING},
          {"void",   TYPE_VOID},
          {"if",     IF},
          {"else",   ELSE},
          {"while",  WHILE},
          {"return", RETURN},
          {"print",  PRINT},
          {"fn",     FN},
          {"let",    LET},
          {"struct", STRUCT}
  };

  char advance();
  bool isAtEnd();
  bool match(char expected);
  static bool isDigit(char c);
  static bool isAlphabet(char c);
  static bool isAlphabetNumeric(char c);

  void addToken(TokenKind type);
  void string();
  void number();
  void identifier();

  void scanToken();
public:
  explicit Lexer(const std::string &source);
  std::vector<TokenInfo> scanTokens();
  void dump() const;
};
}

#endif //TINYCCOMPILER_LEXER_HPP
