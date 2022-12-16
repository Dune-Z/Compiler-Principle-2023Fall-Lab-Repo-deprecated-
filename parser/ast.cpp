#include "ast.hpp"
#include <cassert>

namespace TinyC::Token{
    DumpVisitor::DumpVisitor(std::ostream &out): out{out} {}
    void DumpVisitor::operator()() {
        // TODO: Handling Error.
    }
    void DumpVisitor::operator()(double value) const {
        out << "\033[36mNumberLiteral\033[0m " << value;
    }
    void DumpVisitor::operator()(const std::string &str) const {
        out << "\033[36mStringLiteral\033[0m " << str;
    }
}

namespace TinyC::Expr{
    DumpVisitor::DumpVisitor(std::ostream &out): out{out} {}
    void DumpVisitor::operator()(std::unique_ptr<Literal> literalNode) {
        auto literal = literalNode->getLiteral();
        assert(literal.literal.has_value());
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
    void DumpVisitor::operator()(std::unique_ptr<Unary> unaryNode) {
        auto op = unaryNode->getOperator();
        out << "\033[36mUnaryOperator \033[0m";
        switch (op.type) {
            case Token::TOKEN_OPERATOR_BANG: out << " '!' "; break;
            case Token::TOKEN_OPERATOR_ADD: out << " '+' "; break;
            case Token::TOKEN_OPERATOR_SUB: out << " '-' "; break;
            default:
                // TODO: Handling Error.
                exit(-1);
        }
        out << "<line: " << op.line << ">\n";
    }
}
