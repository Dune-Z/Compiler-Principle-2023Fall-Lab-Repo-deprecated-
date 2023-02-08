#include "Lexer.hpp"
#include <iostream>
namespace tinyc {
Lexer::Lexer(const std::string &source): source(source) {
  start = current = source.begin();
}

void Lexer::dump() const {
  for(const auto& token: tokens)
    std::cout << token << std::endl;
}

char Lexer::advance(){return *current++;}
bool Lexer::isAtEnd() {return *current == *source.end();}
bool Lexer::match(char expected) {
  if(isAtEnd() || *current != expected) return false;
  current++;
  return true;
}
bool Lexer::isDigit(char c) {return c >= '0' && c <= '9';}
bool Lexer::isAlphabet(char c) {return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';}
bool Lexer::isAlphabetNumeric(char c) {return isDigit(c) || isAlphabet(c);}
void Lexer::addToken(TokenKind type) {
  std::string lexeme = std::string{start, current};
  TokenInfo token{currentLine, type, lexeme};
  tokens.push_back(token);
}

void Lexer::string() {
  while(*current != '"' && !isAtEnd()) {
    if(*current == '\n') currentLine++;
    advance();
  }
  if(isAtEnd()){
    throw std::runtime_error("unterminated string.");
  }

  advance();
  addToken(STRING);
}

void Lexer::number() {
  while(isDigit(*current)) advance();
  if(*current == '.' && isDigit(*(current + 1))) {
    advance();
    while (isDigit(*current)) advance();
  }
  addToken(NUMBER);
}

void Lexer::identifier() {
  TokenKind type;
  while(isAlphabetNumeric(*current)) advance();
  std::string text = std::string{start, current};
  auto found = keywords.find(text);
  if(found == keywords.end()) type = IDENTIFIER;
  else type = found->second;
  addToken(type);
}

void Lexer::scanToken() {
  char c = advance();
  switch (c) {
    case '(': addToken(LEFT_PAREN); break;
    case ')': addToken(RIGHT_PAREN); break;
    case '{': addToken(LEFT_BRACE); break;
    case '}': addToken(RIGHT_BRACE); break;
    case '[': addToken(LEFT_BRACKET); break;
    case ']': addToken(RIGHT_BRACKET); break;
    case ',': addToken(COMMA); break;
    case '.': addToken(DOT); break;
    case '+': addToken(OPERATOR_ADD); break;
    case ';': addToken(SEMICOLON); break;
    case ':': addToken(COLON); break;
    case '*': addToken(OPERATOR_MUL); break;
    case '-': addToken(match('>') ? ARROW: OPERATOR_SUB); break;
    case '|': addToken(match('|') ? OPERATOR_OR : ERROR); break;
    case '&': addToken(match('&') ? OPERATOR_AND : REF); break;
    case '!': addToken(match('=') ? OPERATOR_BANG_EQUAL : OPERATOR_BANG); break;
    case '=': addToken(match('=') ? OPERATOR_EQUAL_EQUAL : OPERATOR_EQUAL); break;
    case '<': addToken(match('=') ? OPERATOR_LESS_EQUAL : OPERATOR_LESS); break;
    case '>': addToken(match('=') ? OPERATOR_GREATER_EQUAL : OPERATOR_GREATER); break;
    case '"': string(); break;
    case '/':
      if(match('/')) while (*current != '\n' && !isAtEnd()) advance();
      else addToken(OPERATOR_DIV);
      break;
    case ' ':
    case '\r':
    case '\t':
      break;
    case '\n':
      currentLine++;
      break;
    default:
      if(isDigit(c)) number();
      else if(isAlphabet(c)) identifier();
      else {
        throw std::runtime_error("Undefined character.");
      }
  }
}

std::vector<TokenInfo> Lexer::scanTokens() {
  while(!isAtEnd()){
    start = current;
    scanToken();
  }
  start = current;
  addToken(FEOF);
  return tokens;
}
}

