#ifndef INTERPRETER_PARSER_HPP
#define INTERPRETER_PARSER_HPP
#include "token.hpp"
#include <ostream>
#include <memory>

namespace TinyC::Expr{
    class Expr;
    using ExprObject = std::unique_ptr<Expr>;

    class Expr {
    private:
    public:
    };

    class Literal: public Expr{
    private:
        Token::Token literal;
    public:
        explicit Literal(const Token::Token &literal);
        [[nodiscard]] Token::Token getLiteral() const;
    };

    class Variable: public Expr{
    private:
        Token::Token identifier;
    public:
        explicit Variable(const Token::Token& identifier);
    };

    class Unary: public Expr{
    private:
        Token::Token op;
        ExprObject rhs;
    public:
        Unary(const Token::Token& op, ExprObject rhs);
        [[nodiscard]] Token::Token getOperator() const;
    };

    class Binary: public Expr{
    private:
        Token::Token op;
        ExprObject lhs;
        ExprObject rhs;
    public:
        Binary(const Token::Token& op, ExprObject lhs, ExprObject rhs);
    };

    class Group: public Expr{
    private:
        ExprObject expr;
    public:
        explicit Group(ExprObject expr);
    };
}

namespace TinyC::Stmt{
    class Stmt;
    using StmtObject = std::unique_ptr<Stmt>;

    class Stmt{
    private:
    public:
    };

    class VarDecl: public Stmt{
    private:
        Token::Token type;
        Token::Token variable;
        Expr::ExprObject expr;
    public:
        VarDecl(const Token::Token &type, const Token::Token &variable, Expr::ExprObject expr);
    };

    class FuncDecl: public Stmt{
    private:
        Token::Token type;
        Token::Token function;
        std::vector<StmtObject> functionBlock;
    public:
        FuncDecl(const Token::Token &type, const Token::Token &function, std::vector<StmtObject> functionBlock);
    };

    class IfStmt: public Stmt{
    private:
        Expr::ExprObject condition;
        StmtObject thenBranch;
        StmtObject elseBranch;
    public:
        IfStmt(Expr::ExprObject condition, StmtObject thenBranch, StmtObject elseBranch);
    };

    class WhileStmt: public Stmt{
    private:
        Expr::ExprObject condition;
        StmtObject whileBlock;
    public:
        WhileStmt(Expr::ExprObject condition, StmtObject whileBlock);
    };

    class Block: public Stmt{
    private:
        std::vector<StmtObject> statements;
    public:
        explicit Block(std::vector<StmtObject> statements);
    };
}

namespace TinyC{
    class Parser{
    private:
        std::vector<Token::Token> tokens;
        std::vector<Token::Token>::const_iterator current;

        bool isAtEnd();
        bool check(Token::tokenType type);
        Token::Token consume(Token::tokenType type, std::string&& message);

        Token::Token advance();
        Token::Token previous();
        Token::Token lookahead();

        template<typename Head, typename... Tails>
        bool match(const Head& head, const Tails&... tails);
        template<typename Head>
        bool match(const Head& head);


        Expr::ExprObject Expression();
        Expr::ExprObject LogicOrExpr();
        Expr::ExprObject LogicAndExpr();
        Expr::ExprObject EqualityExpr();
        Expr::ExprObject ComparisonExpr();
        Expr::ExprObject TermExpr();
        Expr::ExprObject FactorExpr();
        Expr::ExprObject UnaryExpr();
        Expr::ExprObject PrimaryExpr();

        Stmt::StmtObject VarDecl();
        Stmt::StmtObject FuncDecl();
        Stmt::StmtObject Stmt();
        Stmt::StmtObject IfStmt();
        Stmt::StmtObject WhileStmt();
        std::vector<Stmt::StmtObject> Block();
    public:
        explicit Parser(const std::vector<Token::Token>& tokens);
        std::vector<Stmt::StmtObject> parse();
    };

    template <typename Head, typename... Tails>
    bool Parser::match(const Head &head, const Tails &...tails) {
        if(check(head)){
            advance();
            return true;
        } else return match(tails...);
    }

    template <typename Head>
    bool Parser::match(const Head &head) {
        if(check(head)){
            advance();
            return true;
        } else return false;
    }
}
#endif //INTERPRETER_PARSER_HPP
