#include "parser.hpp"
#include "token.hpp"
#include <utility>
#include <memory>

namespace TinyC::Expr{
    Literal::Literal(const Token::Token &literal): literal(literal) {}
    Token::Token Literal::getLiteral() const {return literal;}

    Variable::Variable(const Token::Token& identifier): identifier{identifier} {}

    Unary::Unary(const Token::Token& op, ExprObject rhs): op{op}, rhs{std::move(rhs)} {}
    Token::Token Unary::getOperator() const {return op;}

    Binary::Binary(const Token::Token& op, ExprObject lhs, ExprObject rhs): op{op}, lhs{std::move(lhs)}, rhs{std::move(rhs)} {}

    Group::Group(ExprObject expr): expr{std::move(expr)} {}
}

namespace TinyC::Stmt{
    VarDecl::VarDecl(const Token::Token &type, const Token::Token &variable, Expr::ExprObject expr)
    : type{type}, variable{variable}, expr{std::move(expr)} {}
    FuncDecl::FuncDecl(const Token::Token &type, const Token::Token &function, std::vector<StmtObject> functionBlock)
    : type{type}, function{function}, functionBlock{std::move(functionBlock)} {}
    IfStmt::IfStmt(Expr::ExprObject condition, StmtObject thenBranch, StmtObject elseBranch)
    : condition{std::move(condition)}, thenBranch{std::move(thenBranch)}, elseBranch{std::move(elseBranch)} {}
    WhileStmt::WhileStmt(Expr::ExprObject condition, StmtObject whileBlock)
    : condition{std::move(condition)}, whileBlock{std::move(whileBlock)}{}
    Block::Block(std::vector<StmtObject> statements)
    : statements{std::move(statements)} {}
}

namespace TinyC{
    Parser::Parser(const std::vector<Token::Token> &tokens): tokens{tokens} {current = tokens.begin();}
    bool Parser::isAtEnd() {return current == tokens.end();}
    bool Parser::check(Token::tokenType type) {return type == lookahead().type;}
    Token::Token Parser::consume(Token::tokenType type, std::string &&message) {
        if(check(type)) return advance();
        // TODO: Handling Error and Show Message.
        exit(-1);
    }
    Token::Token Parser::advance() {
        if(!isAtEnd()) current++;
        return previous();
    }
    Token::Token Parser::previous() {return *(current - 1);}
    Token::Token Parser::lookahead() {return *current;}

    std::vector<Stmt::StmtObject> Parser::parse() {
        std::vector<Stmt::StmtObject> statements;
        while(!isAtEnd()) statements.push_back(FuncDecl());
        return statements;
    }

    Stmt::StmtObject Parser::VarDecl() {
        Token::Token type = previous();
        Token::Token variable = consume(Token::TOKEN_IDENTIFIER, "Expected variable name after type.");
        Expr::ExprObject initializer;
        if(match(Token::TOKEN_OPERATOR_EQUAL)) initializer = Expression();
        consume(Token::TOKEN_SEMICOLON, "Expected ';' after statement.");
        return std::make_unique<Stmt::VarDecl>(type, variable, std::move(initializer));
    }

    Stmt::StmtObject Parser::FuncDecl() {
        if(!match(Token::TOKEN_TYPE_INT, Token::TOKEN_TYPE_FLOAT, Token::TOKEN_TYPE_STRING, Token::TOKEN_TYPE_BOOLEAN))
            // TODO: Handling Error.
            ;
        Token::Token type = previous();
        Token::Token function = consume(Token::TOKEN_IDENTIFIER, "Expected function name after 'func'.");
        consume(Token::TOKEN_LEFT_PAREN, "Expected '(' after function name.");
        // TODO: Support Passing Arguments to Function.
        consume(Token::TOKEN_RIGHT_PAREN, "Expected ')' after arguments.");
        consume(Token::TOKEN_LEFT_BRACE, "Expected '{' after '('.");
        std::vector<Stmt::StmtObject> block = Block();
        consume(Token::TOKEN_RIGHT_BRACE, "Expected '}' after function block.");

        return std::make_unique<Stmt::FuncDecl>(type, function, std::move(block));
    }

    Stmt::StmtObject Parser::Stmt() {
        if(match(Token::TOKEN_IF)) return IfStmt();
        if(match(Token::TOKEN_WHILE)) return WhileStmt();
        if(match(Token::TOKEN_TYPE_INT, Token::TOKEN_TYPE_FLOAT, Token::TOKEN_TYPE_STRING, Token::TOKEN_TYPE_BOOLEAN)) return VarDecl();
        if(match(Token::TOKEN_LEFT_BRACE)) return std::make_unique<Stmt::Block>(std::move(Block()));
        // TODO: Handling Error.
        exit(-1);
    }

    Stmt::StmtObject Parser::IfStmt() {
        consume(Token::TOKEN_LEFT_PAREN, "Expected '(' after 'if'.");
        Expr::ExprObject condition = Expression();
        consume(Token::TOKEN_RIGHT_PAREN, "Expected ')' after condition.");

        Stmt::StmtObject thenBranch = Stmt();
        Stmt::StmtObject elseBranch;
        if(match(Token::TOKEN_ELSE)) elseBranch = Stmt();
        return std::make_unique<Stmt::IfStmt>(
                std::move(condition),
                std::move(thenBranch),
                std::move(elseBranch)
                );
    }

    Stmt::StmtObject Parser::WhileStmt() {
        consume(Token::TOKEN_LEFT_PAREN, "Expected '(' after 'while'.");
        Expr::ExprObject condition = Expression();
        consume(Token::TOKEN_LEFT_PAREN, "Expected '(' after 'while'.");
        return std::make_unique<Stmt::WhileStmt>(std::move(condition), Stmt());
    }

    std::vector<Stmt::StmtObject> Parser::Block() {
        std::vector<Stmt::StmtObject> statements;
        while(!check(Token::TOKEN_RIGHT_BRACE) && !isAtEnd()) statements.push_back(Stmt());
        consume(Token::TOKEN_RIGHT_PAREN, "Expected '}' after block.");
        return statements;
    }


    Expr::ExprObject Parser::Expression() {
        if(match(Token::TOKEN_STRING))
            return std::make_unique<Expr::Literal>(previous());
        return LogicOrExpr();
    }

    Expr::ExprObject Parser::LogicOrExpr() {
        Expr::ExprObject expr = LogicAndExpr();
        if(match(Token::TOKEN_OPERATOR_OR)) {
            Token::Token op = previous();
            Expr::ExprObject rest = LogicOrExpr();
            expr = std::make_unique<Expr::Binary>(op, std::move(expr), std::move(rest));
        }
        return expr;
    }

    Expr::ExprObject Parser::LogicAndExpr() {
        Expr::ExprObject expr = EqualityExpr();
        if(match(Token::TOKEN_OPERATOR_AND)) {
            Token::Token op = previous();
            Expr::ExprObject rest = LogicAndExpr();
            expr = std::make_unique<Expr::Binary>(op, std::move(expr), std::move(rest));
        }
        return expr;
    }

    Expr::ExprObject Parser::EqualityExpr() {
        Expr::ExprObject expr = ComparisonExpr();
        if(match(Token::TOKEN_OPERATOR_BANG_EQUAL, Token::TOKEN_OPERATOR_EQUAL_EQUAL)) {
            Token::Token op = previous();
            Expr::ExprObject rest = EqualityExpr();
            expr = std::make_unique<Expr::Binary>(op, std::move(expr), std::move(rest));
        }
        return expr;
    }

    Expr::ExprObject Parser::ComparisonExpr() {
        Expr::ExprObject expr = TermExpr();
        if(match(Token::TOKEN_OPERATOR_LESS, Token::TOKEN_OPERATOR_LESS_EQUAL, Token::TOKEN_OPERATOR_GREATER, Token::TOKEN_OPERATOR_GREATER_EQUAL)) {
            Token::Token op = previous();
            Expr::ExprObject rest = ComparisonExpr();
            expr = std::make_unique<Expr::Binary>(op, std::move(expr), std::move(rest));
        }
        return expr;
    }

    Expr::ExprObject Parser::TermExpr() {
        Expr::ExprObject expr = FactorExpr();
        if(match(Token::TOKEN_OPERATOR_ADD, Token::TOKEN_OPERATOR_SUB)) {
            Token::Token op = previous();
            Expr::ExprObject rest = TermExpr();
            expr = std::make_unique<Expr::Binary>(op, std::move(expr), std::move(rest));
        }
        return expr;
    }

    Expr::ExprObject Parser::FactorExpr() {
        Expr::ExprObject expr = UnaryExpr();
        if(match(Token::TOKEN_OPERATOR_MUL, Token::TOKEN_OPERATOR_DIV)) {
            Token::Token op = previous();
            Expr::ExprObject rest = FactorExpr();
            expr = std::make_unique<Expr::Binary>(op, std::move(expr), std::move(rest));
        }
        return expr;
    }

    Expr::ExprObject Parser::UnaryExpr() {
        if(match(Token::TOKEN_OPERATOR_BANG, Token::TOKEN_OPERATOR_ADD, Token::TOKEN_OPERATOR_SUB)) {
            Token::Token op = previous();
            Expr::ExprObject expr = PrimaryExpr();
            return std::make_unique<Expr::Unary>(op, std::move(expr));
        } else return PrimaryExpr();
    }

    Expr::ExprObject Parser::PrimaryExpr() {
        if(match(Token::TOKEN_NUMBER)) return std::make_unique<Expr::Literal>(previous());
        if(match(Token::TOKEN_IDENTIFIER)) return std::make_unique<Expr::Variable>(previous());
        if(match(Token::TOKEN_LEFT_PAREN)) {
            Expr::ExprObject expr = Expression();
            consume(Token::TOKEN_RIGHT_PAREN, "Expected ')' after expression");
            return std::make_unique<Expr::Group>(std::move(expr));
        }
        // TODO: Handling Error.
        exit(-1);
    }
}

