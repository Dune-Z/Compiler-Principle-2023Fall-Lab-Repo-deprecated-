#include "Interpreter.hpp"
#include "Environment.hpp"
#include <utility>

namespace tinyc {
    Interpreter::Interpreter(Parser parser): parser(std::move(parser)) {}

    void Interpreter::interpret() {
        stmts = parser.parse();
        for(auto iter = stmts.begin(); iter != stmts.end(); iter++) {
            auto funcDeclNode = dynamic_cast<FuncDeclStmt*>(*iter);
            if(funcDeclNode == nullptr) {
                auto varDeclNode = dynamic_cast<VarDeclStmt*>(*iter);
                varDeclNode->accept(*this);
            } else {
                environment.setGlobalFunc(
                        funcDeclNode->function.second,
                        int(iter - stmts.begin()),
                        funcDeclNode->function.first);
                if(funcDeclNode->function.second == "main") funcDeclNode->accept(*this);
            }
        }
    }

    Literal Interpreter::evaluate(ASTNode *node) {
        switch (node->nodeType()) {
            case LiteralExprNode: return visit(dynamic_cast<LiteralExpr*>(node));
            case VarExprNode: return visit(dynamic_cast<VarExpr*>(node));
            case CallExprNode: return visit(dynamic_cast<CallExpr*>(node));
            case UnaryExprNode: return visit(dynamic_cast<UnaryExpr*>(node));
            case BinaryExprNode: return visit(dynamic_cast<BinaryExpr*>(node));
            case GroupExprNode: return visit(dynamic_cast<GroupExpr*>(node));
            case FuncDeclStmtNode: return visit(dynamic_cast<FuncDeclStmt*>(node));
            case VarDeclStmtNode: return visit(dynamic_cast<VarDeclStmt*>(node));
            case AssignStmtNode: return visit(dynamic_cast<AssignStmt*>(node));
            case IfStmtNode: return visit(dynamic_cast<IfStmt*>(node));
            case WhileStmtNode: return visit(dynamic_cast<WhileStmt*>(node));
            case BlockStmtNode: return visit(dynamic_cast<BlockStmt*>(node));
            case ReturnStmtNode: return visit(dynamic_cast<ReturnStmt*>(node));
            case PrintStmtNode:return visit(dynamic_cast<PrintStmt*>(node));
            default:
                throw std::runtime_error("No matches ASTNode type.");
        }
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
