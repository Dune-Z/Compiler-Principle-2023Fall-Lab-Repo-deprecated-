#include "Interpreter.hpp"
#include <vector>

namespace tinyc {
    Literal Interpreter::visit(LiteralExpr* exprNode) {
        return exprNode->literal;
    }

    Literal Interpreter::visit(VarExpr* exprNode) {
        return environment.getClosureVar(exprNode->variable);
    }

    Literal Interpreter::visit(CallExpr* exprNode) {
        std::vector<Literal> argValues;
        for(auto &arg: exprNode->args) argValues.push_back(evaluate(arg));
        int index = environment.getGlobalFunc(exprNode->callee);
        auto function = dynamic_cast<FuncDeclStmt*>(stmts[index]);

        auto fork = environment.working;
        environment.working = exprNode->callee;
        if(environment.working == fork) {
            // Case recursion.
            environment.enterClosure();
            for(int i = 0; i < argValues.size(); i++) {
                auto valueInfo = new ValueInfo({
                    function->params[i].first,
                    argValues[i]
                });
                environment.setClosureVar(function->params[i].second, valueInfo);
            }
        } else {
            // Case normal.
            for(int i = 0; i < argValues.size(); i++) {
                environment.setClosureVar(function->params[i].second, argValues[i]);
            }
            environment.enterClosure();
        }
        auto returnValue = evaluate(function);
        environment.exitClosure();
        environment.working = fork;
        return returnValue;
    }

    Literal Interpreter::visit(UnaryExpr* exprNode) {
        UnaryOperationVisitor visitor(exprNode->op);
        return std::visit(visitor, evaluate(exprNode->rhs).value());
    }

    Literal Interpreter::visit(BinaryExpr* exprNode) {
        BinaryOperationVisitor visitor(exprNode->op);
        auto lvalue = evaluate(exprNode->lhs).value();
        auto rvalue = evaluate(exprNode->rhs).value();
        return std::visit(visitor, lvalue, rvalue);
    }

    Literal Interpreter::visit(GroupExpr* exprNode) {
        return evaluate(exprNode->expr);
    }


    UnaryOperationVisitor::UnaryOperationVisitor(const TokenKind &type): type{type} {}
    Literal UnaryOperationVisitor::operator()(int value) const {
        switch (type) {
            case TOKEN_OPERATOR_BANG: return !value;
            case TOKEN_OPERATOR_SUB: return -value;
            default:
                // TODO: Handling Error.
                throw std::runtime_error("");
        }
    }

    Literal UnaryOperationVisitor::operator()(float value) const {
        switch (type) {
            case TOKEN_OPERATOR_SUB: return -value;
            case TOKEN_OPERATOR_BANG:
                throw std::runtime_error("'!' over float number is not allowed");
            default:
                // TODO: Handling Error.
                throw std::runtime_error("");
        }
    }

    Literal UnaryOperationVisitor::operator()(const std::string &str) {
        throw std::runtime_error("'!' or '-' over float number is not allowed");
    }

    BinaryOperationVisitor::BinaryOperationVisitor(const TokenKind &type): type{type} {}

#define OPERATE(op) return lhs op rhs;
    Literal BinaryOperationVisitor::operator()(const int &lhs, const int &rhs) const {
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
                throw std::runtime_error("");
        }
    }

    Literal BinaryOperationVisitor::operator()(const int &lhs, const float &rhs) const {
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
                throw std::runtime_error("");
        }
    }
    Literal BinaryOperationVisitor::operator()(const int &lhs, const std::string &rhs) {
        // TODO: Handle Error.
        throw std::runtime_error("");
    }

    Literal BinaryOperationVisitor::operator()(const float &lhs, const int &rhs) const {
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
                throw std::runtime_error("");
        }
    }
    Literal BinaryOperationVisitor::operator()(const float &lhs, const float &rhs) const {
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
                throw std::runtime_error("");
        }
    }
    Literal BinaryOperationVisitor::operator()(const float &lhs, const std::string &rhs) {
        // TODO: Handle Error.
        throw std::runtime_error("");
    }

    Literal BinaryOperationVisitor::operator()(const std::string &lhs, const int &rhs) {
        // TODO: Handle Error.
        throw std::runtime_error("");
    }
    Literal BinaryOperationVisitor::operator()(const std::string &lhs, const float &rhs) {
        // TODO: Handle Error.
        throw std::runtime_error("");
    }
    Literal BinaryOperationVisitor::operator()(const std::string &lhs, const std::string &rhs) const {
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
                throw std::runtime_error("");
        }
    }
#undef OPERATE
}