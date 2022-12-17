#ifndef INTERPRETER_PARSER_HPP
#define INTERPRETER_PARSER_HPP
#include "token.hpp"
#include <ostream>
#include <memory>
#include <variant>

namespace TinyC::Expr{
    struct Literal;
    struct Variable;
    struct Unary;
    struct Binary;
    struct Group;

    using ExprObject = std::variant<
            std::unique_ptr<Literal>,
            std::unique_ptr<Variable>,
            std::unique_ptr<Unary>,
            std::unique_ptr<Binary>,
            std::unique_ptr<Group>
            >;

    struct Literal{
        Token::Token literal;
        explicit Literal(const Token::Token &literal);
    };

    struct Variable{
        Token::Token identifier;
        explicit Variable(const Token::Token& identifier);
    };

    struct Unary{
        Token::Token op;
        ExprObject rhs;
        Unary(const Token::Token& op, ExprObject rhs);
    };

    struct Binary{
        Token::Token op;
        ExprObject lhs;
        ExprObject rhs;
        Binary(const Token::Token& op, ExprObject lhs, ExprObject rhs);
    };

    struct Group{
        ExprObject expr;
        explicit Group(ExprObject expr);
    };
}

namespace TinyC::Stmt{
    struct FuncDecl;
    struct VarDecl;
    struct IfStmt;
    struct WhileStmt;
    struct AssignStmt;
    struct ReturnStmt;
    struct Block;
    using StmtObject = std::variant<
            std::unique_ptr<FuncDecl>,
            std::unique_ptr<VarDecl>,
            std::unique_ptr<IfStmt>,
            std::unique_ptr<WhileStmt>,
            std::unique_ptr<AssignStmt>,
            std::unique_ptr<ReturnStmt>,
            std::unique_ptr<Block>
            >;

    struct VarDecl{
        Token::Token type;
        Token::Token variable;
        Expr::ExprObject expr;
        VarDecl(const Token::Token &type, const Token::Token &variable, Expr::ExprObject expr);
    };

    struct FuncDecl{
        Token::Token type;
        Token::Token function;
        std::vector<StmtObject> functionBlock;
        FuncDecl(const Token::Token &type, const Token::Token &function, std::vector<StmtObject> functionBlock);
    };

    struct IfStmt{
        Expr::ExprObject condition;
        StmtObject thenBranch;
        StmtObject elseBranch;
        IfStmt(Expr::ExprObject condition, StmtObject thenBranch, StmtObject elseBranch);
    };

    struct WhileStmt{
        Expr::ExprObject condition;
        StmtObject whileBlock;
        WhileStmt(Expr::ExprObject condition, StmtObject whileBlock);
    };

    struct AssignStmt{
        Token::Token identifier;
        Expr::ExprObject expr;
        AssignStmt(const Token::Token &identifier, Expr::ExprObject expr);
    };

    struct ReturnStmt{
        Expr::ExprObject expr;
        explicit ReturnStmt(Expr::ExprObject expr);
    };

    struct Block{
        std::vector<StmtObject> statements;
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
        Stmt::StmtObject AssignStmt();
        Stmt::StmtObject ReturnStmt();
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
