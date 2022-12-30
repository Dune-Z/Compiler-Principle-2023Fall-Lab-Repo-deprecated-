#include "token.hpp"

namespace TinyC::Token{
    Token::Token(int line, token_t type, std::string lexeme, literal_t literal)
    : line{line}, type{type}, lexeme{std::move(lexeme)}, literal{std::move(literal)} {}

    bool Token::operator==(const Token &rhs) const {
        return rhs.line == line && rhs.type == type && rhs.lexeme == lexeme && rhs.literal == literal;
    }
}

