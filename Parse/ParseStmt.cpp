#include "AST/AST.hpp"
#include "Parser.hpp"

namespace tinyc{
    StmtPtr Parser::parseGlobal() {
        if(!match(TOKEN_TYPE_INT, TOKEN_TYPE_BOOLEAN, TOKEN_TYPE_FLOAT, TOKEN_TYPE_STRING)) {
            throw std::runtime_error("Expect type declaration.");
        }
        TokenKind type = previous().type;
        std::string name = consume(TOKEN_IDENTIFIER, "Expect identifier after type.").lexeme;
        if(match(TOKEN_LEFT_PAREN)) {
            return parseFuncDecl(std::pair(type, name));
        }
        else {
            return parseVarDecl(std::pair(type, name));
        }
    }

    StmtPtr Parser::parseFuncDecl(const TypeLexemePair& function) {
        std::vector<TypeLexemePair> params;
        while(!match(TOKEN_RIGHT_PAREN)) {
            if(!match(TOKEN_TYPE_INT, TOKEN_TYPE_BOOLEAN, TOKEN_TYPE_FLOAT, TOKEN_TYPE_STRING)) {
                throw std::runtime_error("Expect type declaration.");
            }
            TokenKind type = previous().type;
            std::string name = consume(TOKEN_IDENTIFIER, "Expect identifier after type.").lexeme;
            params.emplace_back(type, name);
            if(match(TOKEN_RIGHT_PAREN)) break;
            consume(TOKEN_COMMA, "Expect ',' to separate parameters");
        }
        consume(TOKEN_LEFT_BRACE, "Expect '{' after ')'.");
        auto block = parseBlockStmt();
        return new FuncDeclStmt(function, std::move(params), std::move(block));
    }

    StmtPtr Parser::parseVarDecl(const TypeLexemePair& variable) {
        ExprPtr expr = nullptr;
        if(match(TOKEN_OPERATOR_EQUAL)) expr = parseExpr();
        consume(TOKEN_SEMICOLON, "Expect ';' after statement.");
        return new VarDeclStmt(variable, expr);
    }

    StmtPtr Parser::parseVarDecl() {
        TokenKind type = previous().type;
        std::string name = consume(TOKEN_IDENTIFIER, "Expect identifier after type.").lexeme;
        return parseVarDecl(std::pair(type, name));
    }

    StmtPtr Parser::parseStmt() {
        if(match(TOKEN_IF)) return parseIfStmt();
        if(match(TOKEN_WHILE)) return parseWhileStmt();
        if(match(TOKEN_TYPE_INT, TOKEN_TYPE_BOOLEAN, TOKEN_TYPE_FLOAT, TOKEN_TYPE_STRING)) return parseVarDecl();
        if(match(TOKEN_IDENTIFIER)) return parseAssignStmt();
        if(match(TOKEN_RETURN)) return parseReturnStmt();
        if(match(TOKEN_LEFT_BRACE)) return new BlockStmt(parseBlockStmt());
        if(match(TOKEN_PRINT)) return parsePrintStmt();
        throw std::runtime_error("Invalid token in statement.");
    }

    StmtPtr Parser::parseIfStmt() {
        consume(TOKEN_LEFT_PAREN, "Expect '(' after 'if'.");
        auto condition = parseExpr();
        consume(TOKEN_RIGHT_PAREN, "Expect ')' after condition.");

        auto thenBranch = parseStmt();
        StmtPtr elseBranch = nullptr;
        if(match(TOKEN_ELSE)) elseBranch = parseStmt();
        return new IfStmt(condition,
                                        thenBranch,
                                        elseBranch
                                        );
    }

    StmtPtr Parser::parseWhileStmt() {
        consume(TOKEN_LEFT_PAREN, "Expect '(' after 'while'.");
        auto condition = parseExpr();
        consume(TOKEN_RIGHT_PAREN, "Expect ')' after 'while'.");
        return new WhileStmt(condition, parseStmt());
    }

    StmtPtr Parser::parseAssignStmt() {
        std::string variable = previous().lexeme;
        consume(TOKEN_OPERATOR_EQUAL, "Expect '=' after variable in assignment.");
        auto expr = parseExpr();
        consume(TOKEN_SEMICOLON, "Expect ';' after assignment.");
        return new AssignStmt(variable, expr);

    }

    StmtPtr Parser::parseReturnStmt() {
        auto expr = parseExpr();
        consume(TOKEN_SEMICOLON, "Expect ';' after return.");
        return new ReturnStmt(expr);
    }

    StmtPtr Parser::parsePrintStmt() {
        consume(TOKEN_LEFT_PAREN, "Expect '(' after print.");
        auto expr = parseExpr();
        consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
        consume(TOKEN_SEMICOLON, "Expect ';' after print statement.");
        return new PrintStmt(expr);
    }

    std::vector<StmtPtr> Parser::parseBlockStmt() {
        std::vector<StmtPtr> statements;
        while(!check(TOKEN_RIGHT_BRACE) && !isAtEnd()) statements.push_back(parseStmt());
        consume(TOKEN_RIGHT_BRACE, "Expect '}' after block.");
        return statements;
    }
}
