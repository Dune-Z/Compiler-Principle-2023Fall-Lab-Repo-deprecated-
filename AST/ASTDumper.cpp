#include "ASTDumper.hpp"
#include <iostream>
namespace tinyc {
    ASTDumper::ASTDumper(Parser &parser) { stmts = parser.parse(); }

    void ASTDumper::dump() {
        for(auto &stmt: stmts) dumpNode(stmt);
    }

    Literal ASTDumper::dumpNode(ASTNode *node) {
#define CAST_AND_DUMP(type) dynamic_cast<type*>(node)->accept(*this)
        switch (node->nodeType()) {
            case LiteralExprNode: return CAST_AND_DUMP(LiteralExpr);
            case VarExprNode: return CAST_AND_DUMP(VarExpr);
            case CallExprNode: return CAST_AND_DUMP(CallExpr);
            case UnaryExprNode: return CAST_AND_DUMP(UnaryExpr);
            case BinaryExprNode: return CAST_AND_DUMP(BinaryExpr);
            case GroupExprNode: return CAST_AND_DUMP(GroupExpr);
            case FuncDeclStmtNode: return CAST_AND_DUMP(FuncDeclStmt);
            case VarDeclStmtNode: return CAST_AND_DUMP(VarDeclStmt);
            case AssignStmtNode: return CAST_AND_DUMP(AssignStmt);
            case IfStmtNode: return CAST_AND_DUMP(IfStmt);
            case WhileStmtNode: return CAST_AND_DUMP(WhileStmt);
            case ReturnStmtNode: return CAST_AND_DUMP(ReturnStmt);
            case BlockStmtNode: return CAST_AND_DUMP(BlockStmt);
            case PrintStmtNode: return CAST_AND_DUMP(PrintStmt);
            default:
                throw std::runtime_error("Invalid AST Node in dumper.");
        }
#undef CAST_AND_DUMP
    }

    template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
    template<class... Ts> overload(Ts...) -> overload<Ts...>;
    Literal ASTDumper::visit(LiteralExpr *exprNode) {
        std::cout << std::string(indent, '-');
        std::visit(overload{
                [](auto arg){std::cout << "\033[35mLiteralExpr\033[0m " << arg;}
        }, exprNode->literal.literal.value());
        std::cout << "\033[33m<line: " << exprNode->literal.line << ">\033[0m\n";
        return std::nullopt;
    }

    Literal ASTDumper::visit(VarExpr* exprNode) {
        std::cout << std::string(indent, '-');
        std::cout << "\033[35mVarExpr\033[0m " << exprNode->variable.lexeme;
        std::cout << "\033[33m<line: " << exprNode->variable.line << ">\033[0m\n";
        return std::nullopt;
    }

    Literal ASTDumper::visit(CallExpr* exprNode) {
        std::cout << std::string(indent, '-');
        std::cout << "\033[35mCallExpr\033[0m " << exprNode->callee.lexeme;
        std::cout << "\033[33m<line: " << exprNode->callee.line << ">\033[0m\n";
        indent += 1;
        for(auto &arg: exprNode->args) dumpNode(arg);
        indent -= 1;
        return std::nullopt;
    }

    Literal ASTDumper::visit(UnaryExpr* exprNode) {
        std::cout << std::string(indent, '-');
        std::cout << "\033[35mUnaryExpr\033[0m";
        switch (exprNode->op.type) {
            case TOKEN_OPERATOR_BANG: std::cout << " '!' "; break;
            case TOKEN_OPERATOR_SUB: std::cout << " '-' "; break;
            default:
                // TODO: Handling Error.
                throw std::runtime_error("");
        }
        std::cout << "\033[33m<line: " << exprNode->op.line << ">\033[0m\n";
        indent += 1;
        dumpNode(exprNode->rhs);
        indent -= 1;
        return std::nullopt;
    }

    Literal ASTDumper::visit(BinaryExpr* exprNode) {
        std::cout << std::string(indent, '-');
        auto op = exprNode->op;
        std::cout << "\033[35mBinaryExpr\033[0m";
        switch (op.type) {
            case TOKEN_OPERATOR_BANG: std::cout << " '!'"; break;
            case TOKEN_OPERATOR_AND: std::cout << " '&&'"; break;
            case TOKEN_OPERATOR_OR: std::cout << " '||'"; break;
            case TOKEN_OPERATOR_ADD: std::cout << " '+'"; break;
            case TOKEN_OPERATOR_SUB: std::cout << " '-'"; break;
            case TOKEN_OPERATOR_MUL: std::cout << " '*'"; break;
            case TOKEN_OPERATOR_DIV: std::cout << " '/'"; break;
            case TOKEN_OPERATOR_LESS: std::cout << " '<'"; break;
            case TOKEN_OPERATOR_LESS_EQUAL: std::cout << " '<='"; break;
            case TOKEN_OPERATOR_GREATER: std::cout << " '>'"; break;
            case TOKEN_OPERATOR_GREATER_EQUAL: std::cout << " '>='"; break;
            case TOKEN_OPERATOR_EQUAL_EQUAL: std::cout << " '=='"; break;
            case TOKEN_OPERATOR_BANG_EQUAL: std::cout << " '!='"; break;
            default:
                // TODO: Handling Error.
                throw std::runtime_error("");
        }
        std::cout << "\033[33m<line: " << op.line << ">\033[0m\n";
        indent += 1;
        dumpNode(exprNode->lhs);
        dumpNode(exprNode->rhs);
        indent -= 1;
        return std::nullopt;
    }
    Literal ASTDumper::visit(GroupExpr* exprNode) {
        std::cout << std::string(indent, '-');
        std::cout << "\033[35mGroupExpr \033[0m\n";
        indent += 1;
        dumpNode(exprNode->expr);
        indent -= 1;
        return std::nullopt;
    }
    
    Literal ASTDumper::visit(FuncDeclStmt* stmtNode) {
        std::cout << std::string(indent, '-');
        std::cout << "\033[32mFuncDeclStmt\033[0m " << stmtNode->function.second.lexeme;
        switch(stmtNode->function.first){
            case TOKEN_TYPE_INT: std::cout << "\033[32m 'int()'\033[0m"; break;
            case TOKEN_TYPE_FLOAT: std::cout << "\033[32m 'float()'\033[0m"; break;
            case TOKEN_TYPE_BOOLEAN: std::cout << "\033[32m 'boolean()'\033[0m"; break;
            case TOKEN_TYPE_STRING: std::cout << "\033[32m 'string()'\033[0m"; break;
            default:
                // TODO: Handling Error.
                throw std::runtime_error("");
        }
        std::cout << "\033[33m<line: " << stmtNode->function.second.line << ">\033[0m\n";
        indent += 1;
        for(auto &stmt: stmtNode->body) dumpNode(stmt);
        indent -= 1;
        return std::nullopt;
    }

    Literal ASTDumper::visit(VarDeclStmt* stmtNode) {
        std::cout << std::string(indent, '-');
        std::cout << "\033[32mVarDeclStmt\033[0m " << stmtNode->variable.second.lexeme;
        switch(stmtNode->variable.first){
            case TOKEN_TYPE_INT: std::cout << "\033[32m 'int()'\033[0m"; break;
            case TOKEN_TYPE_FLOAT: std::cout << "\033[32m 'float()'\033[0m"; break;
            case TOKEN_TYPE_BOOLEAN: std::cout << "\033[32m 'boolean()'\033[0m"; break;
            case TOKEN_TYPE_STRING: std::cout << "\033[32m 'string()'\033[0m"; break;
            default:
                // TODO: Handling Error.
                throw std::runtime_error("");
        }
        std::cout << "\033[33m<line: " << stmtNode->variable.second.line << ">\033[0m\n";
        indent += 1;
        dumpNode(stmtNode->expr);
        indent -= 1;
        return std::nullopt;
    }
    Literal ASTDumper::visit(AssignStmt* stmtNode) {
        std::cout << std::string(indent, '-');
        std::cout << "\033[32mAssignStmt\033[0m " << stmtNode->variable.lexeme;
        std::cout << "\033[33m<line: " << stmtNode->variable.line << ">\033[0m\n";
        indent += 1;
        dumpNode(stmtNode->expr);
        indent -= 1;
        return std::nullopt;
    }
    Literal ASTDumper::visit(IfStmt* stmtNode) {
        std::cout << std::string(indent, '-');
        std::cout << "\033[32mIfStmt \033[0m\n";
        indent += 1;
        dumpNode(stmtNode->condition);
        dumpNode(stmtNode->thenBranch);
        dumpNode(stmtNode->elseBranch);
        indent -= 1;
        return std::nullopt;
    }
    Literal ASTDumper::visit(WhileStmt* stmtNode) {
        std::cout << std::string(indent, '-');
        std::cout << "\033[32mWhileStmt\033[0m\n";
        indent += 1;
        dumpNode(stmtNode->body);
        indent -= 1;
        return std::nullopt;
    }
    Literal ASTDumper::visit(BlockStmt* stmtNode) {
        std::cout << std::string(indent, '-');
        std::cout << "\033[32mBlockStmt\033[0m\n";
        indent += 1;
        for(auto &stmt: stmtNode->statements) dumpNode(stmt);
        indent -= 1;
        return std::nullopt;
    }
    Literal ASTDumper::visit(ReturnStmt* stmtNode) {
        std::cout << std::string(indent, '-');
        std::cout << "\033[32mReturnStmt\033[0m\n";
        indent += 1;
        dumpNode(stmtNode->expr);
        indent -= 1;
        return std::nullopt;
    }
    Literal ASTDumper::visit(PrintStmt* stmtNode) {
        std::cout << std::string(indent, '-');
        std::cout << "\033[32mPrintStmt\033[0m\n";
        indent += 1;
        dumpNode(stmtNode->expr);
        indent -= 1;
        return std::nullopt;
    }
}