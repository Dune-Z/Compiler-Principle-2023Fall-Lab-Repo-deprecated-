#include "ast_dumper.hpp"
namespace TinyC::Token{
    DumpVisitor::DumpVisitor(std::ostream &out): out{out} {}
    void DumpVisitor::operator()() {
        // TODO: Handling Error.
    }
    void DumpVisitor::operator()(int value) const {
        out << "\033[36mIntNumberLiteral\033[0m " << value;
    }
    void DumpVisitor::operator()(float value) const {
        out << "\033[36mFloatNumberLiteral\033[0m " << value;
    }
    void DumpVisitor::operator()(const std::string &str) const {
        out << "\033[36mStringLiteral\033[0m " << str;
    }
}

namespace TinyC::Expr{
    DumpVisitor::DumpVisitor(std::ostream &out): out{out} {}

    void DumpVisitor::operator()(std::unique_ptr<Literal> &literalObject) {
        auto literal = literalObject->literal;
        if(!literal.literal.has_value()) {
            // TODO: Handling Error.
            exit(-1);
        }
        std::visit(Token::DumpVisitor(out), literal.literal.value());
        out << "<line: " << literal.line << ">";
        switch(literal.type){
            case Token::TOKEN_TYPE_INT: out << "\033[32m 'int'\033[0m" << std::endl; break;
            case Token::TOKEN_TYPE_FLOAT: out << "\033[32m 'float'\033[0m" << std::endl; break;
            case Token::TOKEN_TYPE_BOOLEAN: out << "\033[32m 'boolean'\033[0m" << std::endl; break;
            case Token::TOKEN_TYPE_STRING: out << "\033[32m 'string'\033[0m" << std::endl; break;
            default:
                // TODO: Handling Error.
                exit(-1);
        }
    }

    void DumpVisitor::operator()(std::unique_ptr<Variable> &varObject) {
        auto identifier = varObject->identifier;
        out << "\033[36mDeclRefExpr \033[0m" << identifier.lexeme << "<line: " << identifier.line << ">\n";
    }

    void DumpVisitor::operator()(std::unique_ptr<Unary> &unaryObject) {
        auto op = unaryObject->op;
        auto &expr = unaryObject->rhs;
        switch (op.type) {
            case Token::TOKEN_OPERATOR_BANG: out << " '!' "; break;
            case Token::TOKEN_OPERATOR_ADD: out << " '+' "; break;
            case Token::TOKEN_OPERATOR_SUB: out << " '-' "; break;
            default:
                // TODO: Handling Error.
                exit(-1);
        }
        out << "<line: " << op.line << ">\n";
        out << ' ';
        std::visit(*this, expr);
    }

    void DumpVisitor::operator()(std::unique_ptr<Binary> &binaryObject) {
        auto op = binaryObject->op;
        auto &lhs = binaryObject->lhs;
        auto &rhs = binaryObject->rhs;
        out << "\033[36mBinaryOperator \033[0m";
        switch (op.type) {
            case Token::TOKEN_OPERATOR_BANG: out << " '!' "; break;
            case Token::TOKEN_OPERATOR_AND: out << " '&&' "; break;
            case Token::TOKEN_OPERATOR_OR: out << " '||' "; break;
            case Token::TOKEN_OPERATOR_ADD: out << " '+' "; break;
            case Token::TOKEN_OPERATOR_SUB: out << " '-' "; break;
            case Token::TOKEN_OPERATOR_MUL: out << " '*' "; break;
            case Token::TOKEN_OPERATOR_DIV: out << " '/' "; break;
            case Token::TOKEN_OPERATOR_LESS: out << " '<' "; break;
            case Token::TOKEN_OPERATOR_LESS_EQUAL: out << " '<=' "; break;
            case Token::TOKEN_OPERATOR_GREATER: out << " '>' "; break;
            case Token::TOKEN_OPERATOR_GREATER_EQUAL: out << " '>=' "; break;
            case Token::TOKEN_OPERATOR_EQUAL_EQUAL: out << " '==' "; break;
            default:
                // TODO: Handling Error.
                exit(-1);
        }
        out << "<line: " << op.line << ">\n";
        out << ' ';
        std::visit(*this, lhs);
        std::visit(*this, rhs);
    }

    void DumpVisitor::operator()(std::unique_ptr<Group> &groupObject) {
        out << "\033[36mGroupExpr\033[0m\n";
    }
}

namespace TinyC::Stmt{
    DumpVisitor::DumpVisitor(std::ostream &out): out{out} {}
    void DumpVisitor::operator()(std::unique_ptr<FuncDecl> &funcDeclObject) {
        out << "Function\n";
    }

    void DumpVisitor::operator()(std::unique_ptr<VarDecl> &varDeclObject) {
        out << "Variable\n";
    }
    void DumpVisitor::operator()(std::unique_ptr<IfStmt> &ifStmtObject) {
        out << "If\n";
    }
    void DumpVisitor::operator()(std::unique_ptr<WhileStmt> &whileStmtObject) {
        out << "While\n";
    }
    void DumpVisitor::operator()(std::unique_ptr<AssignStmt> &assignStmtObject) {
        out << "Assign\n";
    }
    void DumpVisitor::operator()(std::unique_ptr<Block> &blockObject) {
        out << "Block\n";
    }
}
