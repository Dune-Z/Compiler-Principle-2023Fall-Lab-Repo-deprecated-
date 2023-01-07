#include "Parser.hpp"
#include <sstream>
#include <fstream>

namespace tinyc{
    Parser::Parser(const std::string &filename) {
        std::ifstream file(filename);
        std::stringstream buffer;
        buffer << file.rdbuf();
        const std::string source(buffer.str());
        Lexer lexer(source);
        tokens = lexer.scanTokens();
        current = tokens.begin();
    }

    std::vector<StmtPtr> Parser::parse() {
        std::vector<StmtPtr> globals;
        while(!isAtEnd()) globals.push_back(parseGlobal());
        return globals;
    }

    bool Parser::isAtEnd() {return current->type == TOKEN_EOF;}
    bool Parser::check(TokenKind type) {return type == lookahead().type;}
    Token Parser::consume(TokenKind type, std::string &&message) {
        if(check(type)) return advance();
        throw std::runtime_error(message);
    }
    Token Parser::advance() {
        if(!isAtEnd()) current++;
        return previous();
    }
    Token Parser::previous() {return *(current - 1);}
    Token Parser::lookahead() {return *current;}
}