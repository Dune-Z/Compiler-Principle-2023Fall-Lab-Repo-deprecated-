#include "Token.hpp"
#include <utility>
#include <ostream>


namespace tinyc{
    Token::Token(int line, TokenKind type, std::string lexeme, Literal literal):
    line(line), type(type), lexeme(std::move(lexeme)), literal(std::move(literal)) {}

    bool Token::operator==(const Token &token) const {
        return line == token.line && type == token.type && lexeme == token.lexeme && literal == token.literal;
    }

    // WARNING: You don't have to change this code.
    template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
    template<class... Ts> overload(Ts...) -> overload<Ts...>;
    std::ostream& operator<<(std::ostream &out, const Token &token) {
        out << "\033[33m<line:    \033[0m" << token.line << "\033[33m>\033[0m\n";
        out << "\033[33m<type:    \033[0m" << token.type << "\033[33m>\033[0m\n";
        out << "\033[33m<lexeme:  \033[0m" << token.lexeme << "\033[33m>\033[0m\n";
        out << "\033[33m<literal: \033[0m";

        if(token.literal.has_value()) {
            std::visit(overload{
                [&out](auto arg){out << arg << "\033[33m>\033[0m\n";}
                },token.literal.value());

        } else out << "Nan" << "\033[33m>\033[0m\n";
        out << std::endl;
        return out;
    }
}
