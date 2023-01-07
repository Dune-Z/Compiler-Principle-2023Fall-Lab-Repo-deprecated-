#include "Parser.hpp"

namespace tinyc {
    ExprPtr Parser::parseExpr() {
        return parseLogicOrExpr();
    }

    ExprPtr Parser::parseLogicOrExpr() {
        ExprPtr expr = parseLogicAndExpr();
        if(match(TOKEN_OPERATOR_OR)) {
            auto op = previous().type;
            auto rest = parseLogicOrExpr();
            expr = new BinaryExpr(op, expr, rest);
        }
        return expr;
    }

    ExprPtr Parser::parseLogicAndExpr() {
        ExprPtr expr = parseEqualityExpr();
        if(match(TOKEN_OPERATOR_AND)) {
            auto op = previous().type;
            auto rest = parseLogicAndExpr();
            expr = new BinaryExpr(op, expr, rest);
        }
        return expr;
    }

    ExprPtr Parser::parseEqualityExpr() {
        ExprPtr expr = parseComparisonExpr();
        if(match(TOKEN_OPERATOR_BANG_EQUAL, TOKEN_OPERATOR_EQUAL_EQUAL)) {
            auto op = previous().type;
            auto rest = parseEqualityExpr();
            expr = new BinaryExpr(op, expr, rest);
        }
        return expr;
    }

    ExprPtr Parser::parseComparisonExpr() {
        ExprPtr expr = parseTermExpr();
        if(match(TOKEN_OPERATOR_LESS, TOKEN_OPERATOR_LESS_EQUAL, TOKEN_OPERATOR_GREATER, TOKEN_OPERATOR_GREATER_EQUAL)) {
            auto op = previous().type;
            auto rest = parseComparisonExpr();
            expr = new BinaryExpr(op, expr, rest);
        }
        return expr;
    }

    ExprPtr Parser::parseTermExpr() {
        ExprPtr expr = parseFactorExpr();
        if(match(TOKEN_OPERATOR_ADD, TOKEN_OPERATOR_SUB)) {
            auto op = previous().type;
            auto rest = parseTermExpr();
            expr = new BinaryExpr(op, expr, rest);
        }
        return expr;
    }

    ExprPtr Parser::parseFactorExpr() {
        ExprPtr expr = parseUnaryExpr();
        if(match(TOKEN_OPERATOR_MUL, TOKEN_OPERATOR_DIV)) {
            auto op = previous().type;
            auto rest = parseFactorExpr();
            expr = new BinaryExpr(op, expr, rest);
        }
        return expr;
    }

    ExprPtr Parser::parseUnaryExpr() {
        if(match(TOKEN_OPERATOR_BANG, TOKEN_OPERATOR_SUB)) {
            auto op = previous().type;
            auto expr = parseCallExpr();
            return new UnaryExpr(op, expr);
        } else return parseCallExpr();
    }

    ExprPtr Parser::parseCallExpr() {
        if(match(TOKEN_IDENTIFIER)) {
            std::string callee = previous().lexeme;
            if(!match(TOKEN_LEFT_PAREN)) return new VarExpr(callee);
            std::vector<ExprPtr> args;
            while(!match(TOKEN_RIGHT_PAREN)) {
                args.push_back(parseExpr());
                if(match(TOKEN_RIGHT_PAREN)) break;
                consume(TOKEN_COMMA, "Expect ',' to divide argument.");
            }
            return new CallExpr(callee, std::move(args));
        } else {
            return parsePrimaryExpr();
        }
    }

    ExprPtr Parser::parsePrimaryExpr() {
        if(match(TOKEN_NUMBER) || match(TOKEN_STRING)){
            Literal literal = previous().literal;
            return new LiteralExpr(literal);
        }
        if(match(TOKEN_LEFT_PAREN)) {
            auto expr = parseExpr();
            consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression");
            return new GroupExpr(expr);
        }
        throw std::runtime_error("invalid");
    }
}