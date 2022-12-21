#include "ast_evaluator.hpp"
#include "environment.hpp"
#include <iostream>

namespace TinyC::Token{
    void PrintVisitor::operator()(int value) const {
        std::cout << value << std::endl;
    }
    void PrintVisitor::operator()(double value) const {
        std::cout << value << std::endl;
    }
    void PrintVisitor::operator()(const std::string &str) const {
        std::cout << str << std::endl;
    }

    UnaryOperationVisitor::UnaryOperationVisitor(const tokenType &type): type{type} {}
    literal_t UnaryOperationVisitor::operator()(int value) const {
        switch (type) {
            case TOKEN_OPERATOR_BANG: return !value;
            case TOKEN_OPERATOR_SUB: return -value;
            default:
                // TODO: Handling Error.
                throw std::overflow_error("");
        }
    }

    literal_t UnaryOperationVisitor::operator()(double value) const {
        switch (type) {
            case TOKEN_OPERATOR_SUB: return -value;
            case TOKEN_OPERATOR_BANG:
                std::cerr << "'!' over float number is not allowed" << std::endl;
                // TODO: Handling Error.
            default:
                // TODO: Handling Error.
                throw std::overflow_error("");
        }
    }

    literal_t UnaryOperationVisitor::operator()(const std::string &str) {
        // TODO: Handling Error.
        std::cerr << "'!' or '-' over float number is not allowed" << std::endl;
        throw std::overflow_error("");
    }

    BinaryOperationVisitor::BinaryOperationVisitor(const tokenType &type): type{type} {}

#define OPERATE(op) return lhs op rhs;
    literal_t BinaryOperationVisitor::operator()(const int &lhs, const int &rhs) const {
        switch (type) {
            case TOKEN_OPERATOR_AND:
                OPERATE(&&)
            case TOKEN_OPERATOR_OR:
                OPERATE(||)
            case TOKEN_OPERATOR_EQUAL_EQUAL:
                OPERATE(==)
            case TOKEN_OPERATOR_BANG_EQUAL:
                OPERATE(!=)
            case TOKEN_OPERATOR_LESS:
                OPERATE(<)
            case TOKEN_OPERATOR_LESS_EQUAL:
                OPERATE(<=)
            case TOKEN_OPERATOR_GREATER:
                OPERATE(>)
            case TOKEN_OPERATOR_GREATER_EQUAL:
                OPERATE(>=)
            case TOKEN_OPERATOR_ADD:
                OPERATE(+)
            case TOKEN_OPERATOR_SUB:
                OPERATE(-)
            case TOKEN_OPERATOR_MUL:
                OPERATE(*)
            case TOKEN_OPERATOR_DIV:
                OPERATE(/)
            default:
                // TODO: Handle Error.
                throw std::overflow_error("");
        }
    }

    literal_t BinaryOperationVisitor::operator()(const int &lhs, const double &rhs) const {
        switch (type) {
            case TOKEN_OPERATOR_EQUAL_EQUAL:
                OPERATE(==)
            case TOKEN_OPERATOR_BANG_EQUAL:
                OPERATE(!=)
            case TOKEN_OPERATOR_LESS:
                OPERATE(<)
            case TOKEN_OPERATOR_LESS_EQUAL:
                OPERATE(<=)
            case TOKEN_OPERATOR_GREATER:
                OPERATE(>)
            case TOKEN_OPERATOR_GREATER_EQUAL:
                OPERATE(>=)
            case TOKEN_OPERATOR_ADD:
                OPERATE(+)
            case TOKEN_OPERATOR_SUB:
                OPERATE(-)
            case TOKEN_OPERATOR_MUL:
                OPERATE(*)
            case TOKEN_OPERATOR_DIV:
                OPERATE(/)
            default:
                // TODO: Handle Error.
                throw std::overflow_error("");
        }
    }
    literal_t BinaryOperationVisitor::operator()(const int &lhs, const std::string &rhs) {
        // TODO: Handle Error.
        throw std::overflow_error("");
    }

    literal_t BinaryOperationVisitor::operator()(const double &lhs, const int &rhs) const {
        switch (type) {
            case TOKEN_OPERATOR_EQUAL_EQUAL:
                OPERATE(==)
            case TOKEN_OPERATOR_BANG_EQUAL:
                OPERATE(!=)
            case TOKEN_OPERATOR_LESS:
                OPERATE(<)
            case TOKEN_OPERATOR_LESS_EQUAL:
                OPERATE(<=)
            case TOKEN_OPERATOR_GREATER:
                OPERATE(>)
            case TOKEN_OPERATOR_GREATER_EQUAL:
                OPERATE(>=)
            case TOKEN_OPERATOR_ADD:
                OPERATE(+)
            case TOKEN_OPERATOR_SUB:
                OPERATE(-)
            case TOKEN_OPERATOR_MUL:
                OPERATE(*)
            case TOKEN_OPERATOR_DIV:
                OPERATE(/)
            default:
                // TODO: Handle Error.
                throw std::overflow_error("");
        }
    }
    literal_t BinaryOperationVisitor::operator()(const double &lhs, const double &rhs) const {
        switch (type) {
            case TOKEN_OPERATOR_EQUAL_EQUAL:
                OPERATE(==)
            case TOKEN_OPERATOR_BANG_EQUAL:
                OPERATE(!=)
            case TOKEN_OPERATOR_LESS:
                OPERATE(<)
            case TOKEN_OPERATOR_LESS_EQUAL:
                OPERATE(<=)
            case TOKEN_OPERATOR_GREATER:
                OPERATE(>)
            case TOKEN_OPERATOR_GREATER_EQUAL:
                OPERATE(>=)
            case TOKEN_OPERATOR_ADD:
                OPERATE(+)
            case TOKEN_OPERATOR_SUB:
                OPERATE(-)
            case TOKEN_OPERATOR_MUL:
                OPERATE(*)
            case TOKEN_OPERATOR_DIV:
                OPERATE(/)
            default:
                // TODO: Handle Error.
                throw std::overflow_error("");
        }
    }
    literal_t BinaryOperationVisitor::operator()(const double &lhs, const std::string &rhs) {
        // TODO: Handle Error.
        throw std::overflow_error("");
    }

    literal_t BinaryOperationVisitor::operator()(const std::string &lhs, const int &rhs) {
        // TODO: Handle Error.
        throw std::overflow_error("");
    }
    literal_t BinaryOperationVisitor::operator()(const std::string &lhs, const double &rhs) {
        // TODO: Handle Error.
        throw std::overflow_error("");
    }
    literal_t BinaryOperationVisitor::operator()(const std::string &lhs, const std::string &rhs) const {
        switch (type) {
            case TOKEN_OPERATOR_EQUAL_EQUAL:
                OPERATE(==)
            case TOKEN_OPERATOR_BANG_EQUAL:
                OPERATE(!=)
            case TOKEN_OPERATOR_LESS:
                OPERATE(<)
            case TOKEN_OPERATOR_LESS_EQUAL:
                OPERATE(<=)
            case TOKEN_OPERATOR_GREATER:
                OPERATE(>)
            case TOKEN_OPERATOR_GREATER_EQUAL:
                OPERATE(>=)
            case TOKEN_OPERATOR_ADD:
                OPERATE(+)
            default:
                // TODO: Handle Error.
                throw std::overflow_error("");
        }
    }
#undef OPERATE
}

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
        Token::UnaryOperationVisitor visitor{unaryObject->op.type};
        return std::visit(visitor, rhsExpr.value());
    }

    Token::literal_t EvaluateVisitor::operator()(std::unique_ptr<Binary> &binaryObject) {
        Token::BinaryOperationVisitor visitor{binaryObject->op.type};
        auto lhsExpr = std::visit(*this, binaryObject->lhs);
        auto rhsExpr = std::visit(*this, binaryObject->rhs);
        return std::visit(visitor, lhsExpr.value(), rhsExpr.value());
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
                        std::get<int>(expr.value()),
                        type)});
                break;
            case Token::TOKEN_TYPE_BOOLEAN:
                table.insert({varDeclObject->variable.lexeme, std::make_pair(
                        bool(std::get<int>(expr.value())),
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
        if(std::get<int>(condition.value())) std::visit(*this, ifStmtObject->thenBranch);
        else if(ifStmtObject->elseBranch.has_value()) std::visit(*this, ifStmtObject->elseBranch.value());
        else;
    }

    void EvaluateVisitor::operator()(std::unique_ptr<WhileStmt> &whileStmtObject) {
        while(auto condition = std::visit(Expr::EvaluateVisitor{table}, whileStmtObject->condition)) {
            if(!condition.has_value()){
                // TODO: Handling Error.
                throw std::overflow_error("");
            }
            if(!std::get<int>(condition.value())) break;
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

    void EvaluateVisitor::operator()(std::unique_ptr<PrintStmt> &printObject) const {
        Expr::EvaluateVisitor visitor{table};
        auto literal = std::visit(visitor, printObject->expr);
        if(!literal.has_value()){
            // TODO: Handling Error.
            throw std::overflow_error("");
        }
        Token::PrintVisitor printer;
        std::visit(printer, literal.value());
    }
}