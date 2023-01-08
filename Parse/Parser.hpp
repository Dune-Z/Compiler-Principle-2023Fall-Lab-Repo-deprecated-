#ifndef TINYC_PARSER_HPP
#define TINYC_PARSER_HPP
#include "Lex/Lexer.hpp"
#include "Lex/Token.hpp"
#include "AST/AST.hpp"
#include <vector>
namespace tinyc{
    class Parser {
    private:
        std::vector<Token> tokens;
        std::vector<Token>::const_iterator current;
        bool isAtEnd();
        bool check(TokenKind type);
        Token consume(TokenKind type, std::string&& message);

        Token advance();
        Token previous();
        Token lookahead();

        template<typename Head, typename... Tails>
        bool match(const Head& head, const Tails&... tails);
        template<typename Head>
        bool match(const Head& head);

        ExprPtr parseExpr();
        ExprPtr parseLogicOrExpr();
        ExprPtr parseLogicAndExpr();
        ExprPtr parseEqualityExpr();
        ExprPtr parseComparisonExpr();
        ExprPtr parseTermExpr();
        ExprPtr parseFactorExpr();
        ExprPtr parseUnaryExpr();
        ExprPtr parseCallExpr();
        ExprPtr parseCallExpr(const std::string &callee);
        ExprPtr parsePrimaryExpr();

        StmtPtr parseGlobal();
        StmtPtr parseVarDecl();
        StmtPtr parseVarDecl(const TypeLexemePair& variable);
        StmtPtr parseFuncDecl(const TypeLexemePair& function);
        StmtPtr parseStmt();
        StmtPtr parseIfStmt();
        StmtPtr parseWhileStmt();
        StmtPtr parseAssignStmt(const std::string &variable);
        StmtPtr parseReturnStmt();
        StmtPtr parsePrintStmt();
        std::vector<StmtPtr> parseBlockStmt();

    public:
        explicit Parser(const std::string &filename);
        std::vector<StmtPtr> parse();
    };

    template <typename Head, typename... Tails>
    bool Parser::match(const Head &head, const Tails &...tails) {
        if(check(head)){
            advance();
            return true;
        } else return match(tails...);
    }

    template <typename Head>
    bool Parser::match(const Head &head) {
        if(check(head)){
            advance();
            return true;
        } else return false;
    }
}

#endif //TINYC_PARSER_HPP
