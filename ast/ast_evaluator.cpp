#include "ast_evaluator.hpp"
#include "environment.hpp"
#include <iostream>

namespace TinyC::Expr{
    EvaluateVisitor::EvaluateVisitor(const table_t &table): table{table} {}

    Token::literal_t EvaluateVisitor::operator()(std::unique_ptr<Literal> &literalObject) {
        auto literal = literalObject->literal.literal;
        if(!literal.has_value()){
            // TODO: Handle Error.
        }
        return literal;
    }

    Token::literal_t EvaluateVisitor::operator()(std::unique_ptr<Variable> &varObject) {
        auto found = table.find(varObject->identifier.lexeme);
        if(found == table.end()) {
            // TODO: Handling Error.
            //  the identifier must be assigned in VarDecl() or AssignStmt() prior to this.
        }
        auto value = found->second;
        if(!value.first.has_value()) {
            // TODO: Handling Error.
        }
        return value.first;
    }

    Token::literal_t EvaluateVisitor::operator()(std::unique_ptr<Unary> &unaryObject) {
        auto rhsExpr = std::visit(*this, unaryObject->rhs);
        switch(unaryObject->op.type) {
            case Token::TOKEN_OPERATOR_BANG: return !std::get<bool>(rhsExpr.value());
            case Token::TOKEN_OPERATOR_SUB: return -std::get<double>(rhsExpr.value());
            default:
                // TODO: Handing Error.
                throw std::overflow_error("");
        }

    }

    Token::literal_t EvaluateVisitor::operator()(std::unique_ptr<Binary> &binaryObject) {
        auto lhsExpr = std::visit(*this, binaryObject->lhs);
        auto rhsExpr = std::visit(*this, binaryObject->rhs);
        switch (binaryObject->op.type) {
            case Token::TOKEN_OPERATOR_AND:
                return bool(std::get<double>(lhsExpr.value())) && bool(std::get<double>(rhsExpr.value()));
            case Token::TOKEN_OPERATOR_OR:
                return bool(std::get<double>(lhsExpr.value())) || bool(std::get<double>(rhsExpr.value()));
            case Token::TOKEN_OPERATOR_EQUAL_EQUAL:
                return std::get<double>(lhsExpr.value()) == std::get<double>(rhsExpr.value());
            case Token::TOKEN_OPERATOR_BANG_EQUAL:
                return std::get<double>(lhsExpr.value()) != std::get<double>(rhsExpr.value());
            case Token::TOKEN_OPERATOR_LESS:
                return std::get<double>(lhsExpr.value()) < std::get<double>(rhsExpr.value());
            case Token::TOKEN_OPERATOR_LESS_EQUAL:
                return std::get<double>(lhsExpr.value()) <= std::get<double>(rhsExpr.value());
            case Token::TOKEN_OPERATOR_GREATER:
                return std::get<double>(lhsExpr.value()) > std::get<double>(rhsExpr.value());
            case Token::TOKEN_OPERATOR_GREATER_EQUAL:
                return std::get<double>(lhsExpr.value()) >= std::get<double>(rhsExpr.value());
            case Token::TOKEN_OPERATOR_ADD:
                return std::get<double>(lhsExpr.value()) + std::get<double>(rhsExpr.value());
            case Token::TOKEN_OPERATOR_SUB:
                return std::get<double>(lhsExpr.value()) - std::get<double>(rhsExpr.value());
            case Token::TOKEN_OPERATOR_MUL:
                return std::get<double>(lhsExpr.value()) * std::get<double>(rhsExpr.value());
            case Token::TOKEN_OPERATOR_DIV:
                return std::get<double>(lhsExpr.value()) / std::get<double>(rhsExpr.value());
            default:
                // TODO: Handle Error.
                throw std::overflow_error("");
        }
    }
    Token::literal_t EvaluateVisitor::operator()(std::unique_ptr<Group> &groupObject) {
        return std::visit(*this, groupObject->expr);
    }
}

namespace TinyC::Stmt{
    void EvaluateVisitor::operator()(std::unique_ptr<FuncDecl> &funcDeclObject) {
        // TODO: Support Function.
        for(auto &stmt: funcDeclObject->functionBlock)
            std::visit(*this, stmt);
    }

    void EvaluateVisitor::operator()(std::unique_ptr<VarDecl> &varDeclObject) {
        auto find = table.find(varDeclObject->variable.lexeme);
        if(find != table.end()){
            // TODO: Handling Error.
            //  support environment scope, variable in different scope can have same name.
            throw std::overflow_error("");
        }
        Expr::EvaluateVisitor visitor{table};
        auto type = varDeclObject->type.type;
        auto expr = std::visit(visitor, varDeclObject->expr);
        switch (type) {
            case Token::TOKEN_TYPE_STRING:
                table.insert({varDeclObject->variable.lexeme, std::make_pair(expr, type)});
                break;
            case Token::TOKEN_TYPE_INT:
                table.insert({varDeclObject->variable.lexeme, std::make_pair(
                        int(std::get<double>(expr.value())),
                        type)});
                break;
            case Token::TOKEN_TYPE_BOOLEAN:
                table.insert({varDeclObject->variable.lexeme, std::make_pair(
                        bool(std::get<double>(expr.value())),
                        type)});
                break;
            case Token::TOKEN_TYPE_FLOAT:
                table.insert({varDeclObject->variable.lexeme, std::make_pair(expr, type)});
                break;
            default:
                // TODO: Handling Error.
                throw std::overflow_error("");
        }
    }

    void EvaluateVisitor::operator()(std::unique_ptr<IfStmt> &ifStmtObject) {
        Expr::EvaluateVisitor visitor{table};
        auto condition = std::visit(visitor, ifStmtObject->condition);
        if(!condition.has_value()){
            // TODO: Handling Error.
            throw std::overflow_error("");
        }
        if(std::get<bool>(condition.value())) std::visit(*this, ifStmtObject->thenBranch);
        else if(ifStmtObject->elseBranch.has_value()) std::visit(*this, ifStmtObject->elseBranch.value());
        else;
    }

    void EvaluateVisitor::operator()(std::unique_ptr<WhileStmt> &whileStmtObject) {
        while(auto condition = std::visit(Expr::EvaluateVisitor{table}, whileStmtObject->condition)) {
            if(!condition.has_value()){
                // TODO: Handling Error.
                throw std::overflow_error("");
            }
            if(!std::get<bool>(condition.value())) break;
            else std::visit(*this, whileStmtObject->whileBlock);
        }
    }

    void EvaluateVisitor::operator()(std::unique_ptr<AssignStmt> &assignStmtObject) {
        auto find = table.find(assignStmtObject->identifier.lexeme);
        if(find == table.end()) {
            // TODO: Handling Error.
            throw std::overflow_error("");
        }
        Expr::EvaluateVisitor visitor{table};
        auto value = std::visit(visitor, assignStmtObject->expr);
        // TODO: Add Type Check.
        find->second.first = value;
    }

    void EvaluateVisitor::operator()(std::unique_ptr<ReturnStmt> &returnStmtObject) {
        // TODO: Support Function Return.
    }

    void EvaluateVisitor::operator()(std::unique_ptr<Block> &blockObject) {
        for(auto &stmt: blockObject->statements)
            std::visit(*this, stmt);
    }
}