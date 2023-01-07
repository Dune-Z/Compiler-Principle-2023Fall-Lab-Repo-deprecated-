#ifndef TINYC_TOKEN_HPP
#define TINYC_TOKEN_HPP
#include <variant>
#include <optional>
#include <string>
#include <ostream>

namespace tinyc{
    using Literal = std::optional<std::variant<int, float, std::string>>;
    enum TokenKind {
        // Literals
        TOKEN_NUMBER, TOKEN_STRING, TOKEN_IDENTIFIER,
        // Type keywords
        TOKEN_TYPE_INT, TOKEN_TYPE_FLOAT, TOKEN_TYPE_BOOLEAN, TOKEN_TYPE_STRING,
        // Operator keywords
        TOKEN_OPERATOR_AND, TOKEN_OPERATOR_OR, TOKEN_OPERATOR_BANG,
        TOKEN_OPERATOR_EQUAL, TOKEN_OPERATOR_BANG_EQUAL, TOKEN_OPERATOR_EQUAL_EQUAL,
        TOKEN_OPERATOR_LESS, TOKEN_OPERATOR_LESS_EQUAL,
        TOKEN_OPERATOR_GREATER, TOKEN_OPERATOR_GREATER_EQUAL,
        TOKEN_OPERATOR_ADD, TOKEN_OPERATOR_SUB, TOKEN_OPERATOR_MUL, TOKEN_OPERATOR_DIV,
        // Control flow keywords
        TOKEN_IF, TOKEN_ELSE, TOKEN_WHILE, TOKEN_RETURN,
        // Others
        TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN, TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
        TOKEN_COMMA, TOKEN_SEMICOLON, TOKEN_DOT,
        TOKEN_PRINT,
        TOKEN_EOF, TOKEN_ERROR
    };

    struct Token {
        int line;
        TokenKind type;
        std::string lexeme;
        Literal literal;
        Token(int line, TokenKind type, std::string lexeme, Literal literal);
        bool operator==(const Token &token) const;
        friend std::ostream& operator<<(std::ostream &out, const Token &token);
    };
}

#endif //TINYC_TOKEN_HPP
