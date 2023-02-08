#ifndef TINYCCOMPILER_TOKENS_HPP
#define TINYCCOMPILER_TOKENS_HPP
#include <string>
#include <ostream>
namespace tinyc {
enum TokenKind {
  // Literals
  NUMBER, STRING, IDENTIFIER,
  // Types
  TYPE_INT, TYPE_FLOAT, TYPE_BOOLEAN, TYPE_STRING, TYPE_VOID,
  // Decl Flags
  FN, LET, STRUCT,
  // Operators
  OPERATOR_AND, OPERATOR_OR, OPERATOR_BANG,
  OPERATOR_EQUAL, OPERATOR_BANG_EQUAL, OPERATOR_EQUAL_EQUAL,
  OPERATOR_LESS, OPERATOR_LESS_EQUAL,
  OPERATOR_GREATER, OPERATOR_GREATER_EQUAL,
  OPERATOR_ADD, OPERATOR_SUB, OPERATOR_MUL, OPERATOR_DIV,
  // Control flow keywords
  IF, ELSE, WHILE, RETURN,
  // Punctuations
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET, RIGHT_BRACKET,
  COMMA, SEMICOLON, DOT, ARROW, COLON,
  // Args Flag.
  REF,
  // Built-in functions
  PRINT,
  // Scan Flags
  FEOF, ERROR
};

class TokenInfo {
private:
  int line;
  // TODO: location with more detail, currently just line num.
  //  Location class contains row, column position and source name with tracing method.
  TokenKind kind;
  std::string lexeme;
public:
  TokenInfo(int line, TokenKind kind, std::string lexeme);
  friend std::ostream& operator<<(std::ostream &out, const TokenInfo &tokenInfo);
  int getLocation() const {return line;}
  TokenKind getKind() const {return kind;}
  std::string getLexeme() const {return lexeme;}
};
}

#endif //TINYCCOMPILER_TOKENS_HPP
