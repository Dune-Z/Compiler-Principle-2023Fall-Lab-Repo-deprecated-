#ifndef SCANNER_HPP
#define SCANNER_HPP
#include <vector>
#include <unordered_map>
#include "parser/token.hpp"

namespace TinyC::Token{
    class Scanner{
    public:
        explicit Scanner(const std::string& source);
        std::vector<Token> scanTokens();
    private:
        std::string source;
        std::vector<Token> tokens;

        int line = 0;
        std::string::const_iterator start;
        std::string::const_iterator current;
        std::unordered_map<std::string, tokenType> keywords;

        char advance();
        bool isAtEnd();
        bool match(char expected);
        static bool isDigit(char c);
        static bool isAlphabet(char c);
        static bool isAlphabetNumeric(char c);

        void addToken(tokenType type);
        void addToken(tokenType type, const literal_t& literal);
        void string();
        void number();
        void identifier();

        void scanToken();
    };
}
#endif

