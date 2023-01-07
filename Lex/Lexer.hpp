#ifndef TINYC_LEXER_HPP
#define TINYC_LEXER_HPP
#include "Token.hpp"
#include <string>
#include <vector>
#include <unordered_map>

namespace tinyc{
    class Lexer {
    private:
        int line;
        std::string source;
        std::vector<Token> tokens;
        std::string::const_iterator start;
        std::string::const_iterator current;
        std::unordered_map<std::string, TokenKind> keywords;
        char advance();
        bool isAtEnd();
        bool match(char expected);
        static bool isDigit(char c);
        static bool isAlphabet(char c);
        static bool isAlphabetNumeric(char c);

        void addToken(TokenKind type);
        void addToken(TokenKind type, const Literal & literal);
        void string();
        void number();
        void identifier();

        void scanToken();
    public:
        explicit Lexer(const std::string &source);
        std::vector<Token> scanTokens();
    };
}

#endif //TINYC_LEXER_HPP
