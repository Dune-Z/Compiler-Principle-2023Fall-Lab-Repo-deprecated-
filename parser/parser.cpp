#include "parser.hpp"
#include "scanner.hpp"
#include "token.hpp"
#include <utility>
#include <memory>

namespace TinyC::Expr{
    Literal::Literal(const Token::Token &literal): literal(literal) {}
    Variable::Variable(const Token::Token& identifier): identifier{identifier} {}
    Call::Call(bool isCall, const Token::Token &callee, args_call_t args): isCall{isCall}, callee{callee}, args{std::move(args)} {}
    Unary::Unary(const Token::Token& op, ExprObject rhs): op{op}, rhs{std::move(rhs)} {}
    Binary::Binary(const Token::Token& op, ExprObject lhs, ExprObject rhs): op{op}, lhs{std::move(lhs)}, rhs{std::move(rhs)} {}
    Group::Group(ExprObject expr): expr{std::move(expr)} {}

}

namespace TinyC::Stmt{
    VarDecl::VarDecl(const Token::Token &type, const Token::Token &variable, Expr::ExprObject expr)
    : type{type}, variable{variable}, expr{std::move(expr)} {}

    FuncDecl::FuncDecl(const Token::Token &type, const Token::Token &function, args_decl_t args, std::vector<StmtObject> functionBlock)
    : type{type}, function{function}, args{std::move(args)}, functionBlock{std::move(functionBlock)} {}

    IfStmt::IfStmt(Expr::ExprObject condition, StmtObject thenBranch, std::optional<StmtObject> elseBranch)
    : condition{std::move(condition)}, thenBranch{std::move(thenBranch)}, elseBranch{std::move(elseBranch)} {}

    WhileStmt::WhileStmt(Expr::ExprObject condition, StmtObject whileBlock)
    : condition{std::move(condition)}, whileBlock{std::move(whileBlock)}{}

    AssignStmt::AssignStmt(const Token::Token &identifier, Expr::ExprObject expr)
    : identifier{identifier}, expr{std::move(expr)} {}

    ReturnStmt::ReturnStmt(Expr::ExprObject expr)
    : expr{std::move(expr)} {}

    Block::Block(std::vector<StmtObject> statements)
    : statements{std::move(statements)} {}

    PrintStmt::PrintStmt(Expr::ExprObject expr)
    : expr{std::move(expr)} {}
}

namespace TinyC{
    Parser::Parser(const std::string &source) {
        Token::Scanner scanner{source};
        tokens = scanner.scanTokens();
        current = tokens.begin();
    }

    bool Parser::isAtEnd() {return current->type == Token::TOKEN_EOF;}
    bool Parser::check(Token::token_t type) {return type == lookahead().type;}
    Token::Token Parser::consume(Token::token_t type, std::string &&message) {
        if(check(type)) return advance();
        // TODO: Handling Error and Show Message.
        throw std::overflow_error(message);
    }
    Token::Token Parser::advance() {
        if(!isAtEnd()) current++;
        return previous();
    }
    Token::Token Parser::previous() {return *(current - 1);}
    Token::Token Parser::lookahead() {return *current;}

    std::vector<Stmt::StmtObject> Parser::parse() {
        std::vector<Stmt::StmtObject> statements;
        while(!isAtEnd()) {
            statements.push_back(FuncDecl());
        }
        return statements;
    }

    Stmt::StmtObject Parser::FuncDecl() {
        if(!match(Token::TOKEN_TYPE_INT, Token::TOKEN_TYPE_FLOAT, Token::TOKEN_TYPE_STRING, Token::TOKEN_TYPE_BOOLEAN))
            // TODO: Handling Error.
            ;
        Token::Token type = previous();
        Token::Token name = consume(Token::TOKEN_IDENTIFIER, "Expect function name after return type.");
        if(!match(Token::TOKEN_LEFT_PAREN)) {
            return VarDecl(type, name);
        }
        Stmt::args_decl_t args;
        while(!match(Token::TOKEN_RIGHT_PAREN)) {
            if(!match(Token::TOKEN_TYPE_INT, Token::TOKEN_TYPE_FLOAT, Token::TOKEN_TYPE_STRING, Token::TOKEN_TYPE_BOOLEAN))
                // TODO: Handling Error.
                ;
            Token::token_t argType = previous().type;
            Token::Token arg = consume(Token::TOKEN_IDENTIFIER, "Expected identifier after type.");
            args.emplace_back(argType, arg);
            if(!match(Token::TOKEN_RIGHT_PAREN))
                consume(Token::TOKEN_COMMA, "Expect ',' to divide arguments");
            else break;
        }
        // consume(Token::TOKEN_RIGHT_PAREN, "Expect ')' after arguments.");
        consume(Token::TOKEN_LEFT_BRACE, "Expect '{' after ')'.");
        std::vector<Stmt::StmtObject> block = Block();

        return std::make_unique<Stmt::FuncDecl>(type, name, args, std::move(block));
    }

    Stmt::StmtObject Parser::VarDecl() {
        Token::Token type = previous();
        Token::Token variable = consume(Token::TOKEN_IDENTIFIER, "Expect variable name after type.");
        return VarDecl(type, variable);
        // TODO: Support list declaration.
    }

    Stmt::StmtObject Parser::VarDecl(const Token::Token &type, const Token::Token &variable) {
        Expr::ExprObject initializer;
        if(match(Token::TOKEN_OPERATOR_EQUAL)) initializer = Expression();
        consume(Token::TOKEN_SEMICOLON, "Expect ';' after statement.");
        return std::make_unique<Stmt::VarDecl>(type, variable, std::move(initializer));
    }

    Stmt::StmtObject Parser::Stmt() {
        if(match(Token::TOKEN_IF)) return IfStmt();
        if(match(Token::TOKEN_WHILE)) return WhileStmt();
        if(match(Token::TOKEN_TYPE_INT, Token::TOKEN_TYPE_FLOAT, Token::TOKEN_TYPE_STRING, Token::TOKEN_TYPE_BOOLEAN)) return VarDecl();
        if(match(Token::TOKEN_IDENTIFIER)) return AssignStmt();
        if(match(Token::TOKEN_RETURN)) return ReturnStmt();
        if(match(Token::TOKEN_LEFT_BRACE)) return std::make_unique<Stmt::Block>(std::move(Block()));
        if(match(Token::TOKEN_PRINT)) return PrintStmt();
        // TODO: Handling Return of function.
        // TODO: Handling Error.
        throw std::overflow_error("");
    }

    Stmt::StmtObject Parser::IfStmt() {
        consume(Token::TOKEN_LEFT_PAREN, "Expect '(' after 'if'.");
        Expr::ExprObject condition = Expression();
        consume(Token::TOKEN_RIGHT_PAREN, "Expect ')' after condition.");

        Stmt::StmtObject thenBranch = Stmt();
        std::optional<Stmt::StmtObject> elseBranch = std::nullopt;
        if(match(Token::TOKEN_ELSE)) elseBranch = Stmt();
        return std::make_unique<Stmt::IfStmt>(
                std::move(condition),
                std::move(thenBranch),
                std::move(elseBranch)
                );
    }

    Stmt::StmtObject Parser::WhileStmt() {
        consume(Token::TOKEN_LEFT_PAREN, "Expect '(' after 'while'.");
        Expr::ExprObject condition = Expression();
        consume(Token::TOKEN_RIGHT_PAREN, "Expect ')' after 'while'.");
        return std::make_unique<Stmt::WhileStmt>(std::move(condition), Stmt());
    }

    Stmt::StmtObject Parser::AssignStmt() {
        Token::Token identifier = previous();
        consume(Token::TOKEN_OPERATOR_EQUAL, "Expect '=' after variable in assignment.");
        Expr::ExprObject expr = Expression();
        consume(Token::TOKEN_SEMICOLON, "Expect ';' after assignment.");
        return std::make_unique<Stmt::AssignStmt>(identifier, std::move(expr));
    }

    Stmt::StmtObject Parser::ReturnStmt() {
        Expr::ExprObject expr = Expression();
        consume(Token::TOKEN_SEMICOLON, "Expect ';' after return.");
        return std::make_unique<Stmt::ReturnStmt>(std::move(expr));
    }

    std::vector<Stmt::StmtObject> Parser::Block() {
        std::vector<Stmt::StmtObject> statements;
        while(!check(Token::TOKEN_RIGHT_BRACE) && !isAtEnd()) statements.push_back(Stmt());
        consume(Token::TOKEN_RIGHT_BRACE, "Expect '}' after block.");
        return statements;
    }

    Stmt::StmtObject Parser::PrintStmt() {
        consume(Token::TOKEN_LEFT_PAREN, "Expect '(' after print.");
        Expr::ExprObject expr = Expression();
        consume(Token::TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
        consume(Token::TOKEN_SEMICOLON, "Expect ';' after print statement.");
        return std::make_unique<Stmt::PrintStmt>(std::move(expr));
    }

    Expr::ExprObject Parser::Expression() {
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
        if(match(Token::TOKEN_OPERATOR_BANG, Token::TOKEN_OPERATOR_SUB)) {
            Token::Token op = previous();
            Expr::ExprObject expr = CallExpr();
            return std::make_unique<Expr::Unary>(op, std::move(expr));
        } else return CallExpr();
    }

    Expr::ExprObject Parser::CallExpr() {
        bool isCall = false;
        Expr::args_call_t args;
        if(match(Token::TOKEN_IDENTIFIER)){
            auto callee = previous();
            if(match(Token::TOKEN_LEFT_PAREN)) {
                isCall = true;
                while(!match(Token::TOKEN_RIGHT_PAREN)){
                    args.push_back(Expression());
                    if(!match(Token::TOKEN_RIGHT_PAREN))
                        consume(Token::TOKEN_COMMA, "Expect ',' to divide argument.");
                    else break;
                }
                return std::make_unique<Expr::Call>(isCall, callee, std::move(args));
            } else return std::make_unique<Expr::Variable>(previous());
        } else return PrimaryExpr();
    }

    Expr::ExprObject Parser::PrimaryExpr() {
        if(match(Token::TOKEN_NUMBER) || match(Token::TOKEN_STRING)) return std::make_unique<Expr::Literal>(previous());
        if(match(Token::TOKEN_LEFT_PAREN)) {
            Expr::ExprObject expr = Expression();
            consume(Token::TOKEN_RIGHT_PAREN, "Expect ')' after expression");
            return std::make_unique<Expr::Group>(std::move(expr));
        }
        // TODO: Handling Error.
        throw std::overflow_error("");
    }
}

