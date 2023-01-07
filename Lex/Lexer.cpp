#include "Lexer.hpp"

namespace tinyc{
    Lexer::Lexer(const std::string &source): source(source) {
        line = 1;
        start = current = source.begin();
        keywords = {
                {"int", TOKEN_TYPE_INT},
                {"bool", TOKEN_TYPE_BOOLEAN},
                {"float", TOKEN_TYPE_FLOAT},
                {"str", TOKEN_TYPE_STRING},
                {"if", TOKEN_IF},
                {"else", TOKEN_ELSE},
                {"while", TOKEN_WHILE},
                {"return", TOKEN_RETURN},
                {"print", TOKEN_PRINT},
        };
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
    void Lexer::addToken(TokenKind type) {addToken(type, std::nullopt);}
    void Lexer::addToken(TokenKind type, const Literal &literal) {
        std::string lexeme = std::string{start, current};
        Token token{line, type, lexeme, literal};
        tokens.push_back(token);
    }

    void Lexer::string() {
        while(*current != '"' && !isAtEnd()) {
            if(*current == '\n') line++;
            advance();
        }
        if(isAtEnd()){
            throw std::overflow_error("[Lexer] Error detected in string tokenization.");
        }

        advance();
        std::string value = std::string{start + 1, current - 1};
        addToken(TOKEN_STRING, value);
    }

    void Lexer::number() {
        Literal value;
        while(isDigit(*current)) advance();
        if(*current == '.' && isDigit(*(current + 1))) {
            advance();
            while (isDigit(*current)) advance();
            value = std::stof(std::string{start, current});
        } else value = std::stoi(std::string(start, current));
        addToken(TOKEN_NUMBER, value);
    }

    void Lexer::identifier() {
        TokenKind type;
        while(isAlphabetNumeric(*current)) advance();
        std::string text = std::string{start, current};
        auto found = keywords.find(text);
        if(found == keywords.end()) type = TOKEN_IDENTIFIER;
        else type = found->second;
        addToken(type);
    }

    void Lexer::scanToken() {
        char c = advance();
        switch (c) {
            case '(': addToken(TOKEN_LEFT_PAREN); break;
            case ')': addToken(TOKEN_RIGHT_PAREN); break;
            case '{': addToken(TOKEN_LEFT_BRACE); break;
            case '}': addToken(TOKEN_RIGHT_BRACE); break;
            case ',': addToken(TOKEN_COMMA); break;
            case '.': addToken(TOKEN_DOT); break;
            case '-': addToken(TOKEN_OPERATOR_SUB); break;
            case '+': addToken(TOKEN_OPERATOR_ADD); break;
            case ';': addToken(TOKEN_SEMICOLON); break;
            case '*': addToken(TOKEN_OPERATOR_MUL); break;
            case '|': addToken(match('|') ? TOKEN_OPERATOR_OR : TOKEN_ERROR); break;
            case '&': addToken(match('&') ? TOKEN_OPERATOR_AND : TOKEN_ERROR); break;
            case '!': addToken(match('=') ? TOKEN_OPERATOR_BANG_EQUAL : TOKEN_OPERATOR_BANG); break;
            case '=': addToken(match('=') ? TOKEN_OPERATOR_EQUAL_EQUAL : TOKEN_OPERATOR_EQUAL); break;
            case '<': addToken(match('=') ? TOKEN_OPERATOR_LESS_EQUAL : TOKEN_OPERATOR_LESS); break;
            case '>': addToken(match('=') ? TOKEN_OPERATOR_GREATER_EQUAL : TOKEN_OPERATOR_GREATER); break;
            case '"': string(); break;
            case '/':
                if(match('/')) while (*current != '\n' && !isAtEnd()) advance();
                else addToken(TOKEN_OPERATOR_DIV);
                break;
            case ' ':
            case '\r':
            case '\t':
                break;
            case '\n':
                line++;
                break;
            default:
                if(isDigit(c)) number();
                else if(isAlphabet(c)) identifier();
                else {
                    // TODO: Handling Error.
                }
        }
    }

    std::vector<Token> Lexer::scanTokens() {
        while(!isAtEnd()){
            start = current;
            scanToken();
        }
        start = current;
        addToken(TOKEN_EOF);
        return tokens;
    }
}
