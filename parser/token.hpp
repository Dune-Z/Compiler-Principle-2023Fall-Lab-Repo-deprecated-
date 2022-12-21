#ifndef TOKEN_HPP
#define TOKEN_HPP
#include <optional>
#include <variant>
#include <vector>
#include <string>

namespace TinyC::Token{
    using literal_t = std::optional<std::variant<int, double, std::string>>;
    enum tokenType{
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

    struct Token{
        int line = 0;
        tokenType type;
        std::string lexeme;
        literal_t literal;
        explicit Token(int line, tokenType type, std::string lexeme, literal_t literal);
        Token& operator=(const Token& rhs) = default;
    };
}


#endif

