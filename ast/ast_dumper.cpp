#include "ast_dumper.hpp"
namespace TinyC::Token{
    DumpVisitor::DumpVisitor(std::ostream &out): out{out} {}

    void DumpVisitor::operator()(int value) const {
        out << "\033[35mNumberLiteral\033[0m " << value;
    }
    void DumpVisitor::operator()(double value) const {
        out << "\033[35mNumberLiteral\033[0m " << value;
    }
    void DumpVisitor::operator()(const std::string &str) const {
        out << "\033[35mStringLiteral\033[0m " << str;
    }
}

namespace TinyC::Expr{
    DumpVisitor::DumpVisitor(std::ostream &out): out{out} {}

    void DumpVisitor::operator()(std::unique_ptr<Literal> &literalObject) {
        auto literal = literalObject->literal;
        if(!literal.literal.has_value()) {
            // TODO: Handling Error.
            throw std::overflow_error("");
        }
        std::visit(Token::DumpVisitor(out), literal.literal.value());
        out << "\033[33m<line: " << literal.line << ">\033[0m\n";
    }

    void DumpVisitor::operator()(std::unique_ptr<Variable> &varObject) {
        auto identifier = varObject->identifier;
        out << "\033[35mIdentifier \033[0m" << identifier.lexeme << "\033[33m<line: " << identifier.line << ">\033[0m\n";
    }

    void DumpVisitor::operator()(std::unique_ptr<Unary> &unaryObject) {
        auto op = unaryObject->op;
        auto &expr = unaryObject->rhs;
        switch (op.type) {
            case Token::TOKEN_OPERATOR_BANG: out << " '!' "; break;
            case Token::TOKEN_OPERATOR_SUB: out << " '-' "; break;
            default:
                // TODO: Handling Error.
                throw std::overflow_error("");
        }
        out << "\033[33m<line: " << op.line << ">\033[0m\n";
        std::visit(*this, expr);
    }

    void DumpVisitor::operator()(std::unique_ptr<Binary> &binaryObject) {
        auto op = binaryObject->op;
        auto &lhs = binaryObject->lhs;
        auto &rhs = binaryObject->rhs;
        out << "\033[36mBinaryOperator \033[0m";
        switch (op.type) {
            case Token::TOKEN_OPERATOR_BANG: out << " '!'"; break;
            case Token::TOKEN_OPERATOR_AND: out << " '&&'"; break;
            case Token::TOKEN_OPERATOR_OR: out << " '||'"; break;
            case Token::TOKEN_OPERATOR_ADD: out << " '+'"; break;
            case Token::TOKEN_OPERATOR_SUB: out << " '-'"; break;
            case Token::TOKEN_OPERATOR_MUL: out << " '*'"; break;
            case Token::TOKEN_OPERATOR_DIV: out << " '/'"; break;
            case Token::TOKEN_OPERATOR_LESS: out << " '<'"; break;
            case Token::TOKEN_OPERATOR_LESS_EQUAL: out << " '<='"; break;
            case Token::TOKEN_OPERATOR_GREATER: out << " '>'"; break;
            case Token::TOKEN_OPERATOR_GREATER_EQUAL: out << " '>='"; break;
            case Token::TOKEN_OPERATOR_EQUAL_EQUAL: out << " '=='"; break;
            case Token::TOKEN_OPERATOR_BANG_EQUAL: out << " '!='"; break;
            default:
                // TODO: Handling Error.
                throw std::overflow_error("");
        }
        out << "\033[33m<line: " << op.line << ">\033[0m\n";
        std::visit(*this, lhs);
        std::visit(*this, rhs);
    }

    void DumpVisitor::operator()(std::unique_ptr<Group> &groupObject) {
        out << "\033[36mGroupExpr\033[0m '()'\n";
        std::visit(*this, groupObject->expr);
    }
}

namespace TinyC::Stmt{
    DumpVisitor::DumpVisitor(std::ostream &out): out{out} {}
    void DumpVisitor::operator()(std::unique_ptr<FuncDecl> &funcDeclObject) {
        auto &blocks = funcDeclObject->functionBlock;
        out << "\033[32mFunctionDecl\033[0m " << funcDeclObject->function.lexeme;
        out << "\033[33m<line: " << funcDeclObject->function.line << ">\033[0m";
        switch(funcDeclObject->type.type){
            case Token::TOKEN_TYPE_INT: out << "\033[32m 'int()'\033[0m" << std::endl; break;
            case Token::TOKEN_TYPE_FLOAT: out << "\033[32m 'float()'\033[0m" << std::endl; break;
            case Token::TOKEN_TYPE_BOOLEAN: out << "\033[32m 'boolean()'\033[0m" << std::endl; break;
            case Token::TOKEN_TYPE_STRING: out << "\033[32m 'string()'\033[0m" << std::endl; break;
            default:
                // TODO: Handling Error.
                throw std::overflow_error("");
        }
        for(auto &block: blocks) std::visit(*this, block);
    }

    void DumpVisitor::operator()(std::unique_ptr<VarDecl> &varDeclObject) {
        out << "\033[32mVariableDecl\033[0m " << varDeclObject->variable.lexeme;
        out << "\033[33m<line: " << varDeclObject->variable.line << ">\033[0m";
        switch(varDeclObject->type.type){
            case Token::TOKEN_TYPE_INT: out << "\033[32m 'int()'\033[0m" << std::endl; break;
            case Token::TOKEN_TYPE_FLOAT: out << "\033[32m 'float()'\033[0m" << std::endl; break;
            case Token::TOKEN_TYPE_BOOLEAN: out << "\033[32m 'boolean()'\033[0m" << std::endl; break;
            case Token::TOKEN_TYPE_STRING: out << "\033[32m 'string()'\033[0m" << std::endl; break;
            default:
                // TODO: Handling Error.
                throw std::overflow_error("");
        }
        Expr::DumpVisitor visitor{out};
        std::visit(visitor, varDeclObject->expr);
    }
    void DumpVisitor::operator()(std::unique_ptr<IfStmt> &ifStmtObject) {
        out << "\033[34mIfStmt\033[0m\n";
        Expr::DumpVisitor visitor{out};
        std::visit(visitor, ifStmtObject->condition);
        std::visit(*this, ifStmtObject->thenBranch);
        if(ifStmtObject->elseBranch.has_value())
            std::visit(*this, ifStmtObject->elseBranch.value());
    }
    void DumpVisitor::operator()(std::unique_ptr<WhileStmt> &whileStmtObject) {
        out << "\033[34mWhileStmt\033[0m\n";
        Expr::DumpVisitor visitor{out};
        std::visit(visitor, whileStmtObject->condition);
        std::visit(*this, whileStmtObject->whileBlock);
    }
    void DumpVisitor::operator()(std::unique_ptr<AssignStmt> &assignStmtObject) {
        out << "\033[34mAssignStmt\033[0m " << assignStmtObject->identifier.lexeme;
        out << "\033[33m<Line: " << assignStmtObject->identifier.line << ">\033[0m\n";
        std::visit(Expr::DumpVisitor{out}, assignStmtObject->expr);
    }
    void DumpVisitor::operator()(std::unique_ptr<ReturnStmt> &returnStmtObject) {
        out << "\033[34mReturnStmt\033[0m\n";
        Expr::DumpVisitor visitor{out};
        std::visit(visitor, returnStmtObject->expr);
    }
    void DumpVisitor::operator()(std::unique_ptr<Block> &blockObject) {
        out << "\033[34mBlock\033[0m\n";
        for(auto &x: blockObject->statements)
            std::visit(*this, x);
    }

    void DumpVisitor::operator()(std::unique_ptr<PrintStmt> &printObject) {
        out << "\033[34mPrintStmt\033[0m\n";
        Expr::DumpVisitor visitor{out};
        std::visit(visitor, printObject->expr);
    }
}
