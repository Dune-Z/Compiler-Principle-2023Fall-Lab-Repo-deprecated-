#ifndef INTERPRETER_PARSER_HPP
#define INTERPRETER_PARSER_HPP
#include "token.hpp"
#include <ostream>
#include <memory>
#include <variant>

namespace TinyC::Expr{
    struct Literal;
    struct Variable;
    struct Call;
    struct Unary;
    struct Binary;
    struct Group;

    using ExprObject = std::variant<
            std::unique_ptr<Literal>,
            std::unique_ptr<Variable>,
            std::unique_ptr<Call>,
            std::unique_ptr<Unary>,
            std::unique_ptr<Binary>,
            std::unique_ptr<Group>
            >;
    using args_call_t = std::vector<ExprObject>;

    struct Literal{
        Token::Token literal;
        explicit Literal(const Token::Token &literal);
    };

    struct Variable{
        Token::Token identifier;
        explicit Variable(const Token::Token& identifier);
    };

    struct Call{
        bool isCall;
        ExprObject callee;
        args_call_t args;
        Call(bool isCall, ExprObject callee, args_call_t args);
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
    struct PrintStmt;
    using StmtObject = std::variant<
            std::unique_ptr<FuncDecl>,
            std::unique_ptr<VarDecl>,
            std::unique_ptr<IfStmt>,
            std::unique_ptr<WhileStmt>,
            std::unique_ptr<AssignStmt>,
            std::unique_ptr<ReturnStmt>,
            std::unique_ptr<Block>,
            std::unique_ptr<PrintStmt>
            >;
    using args_decl_t = std::vector<std::pair<Token::token_t, Token::Token>>;

    struct VarDecl{
        Token::Token type;
        Token::Token variable;
        Expr::ExprObject expr;
        VarDecl(const Token::Token &type, const Token::Token &variable, Expr::ExprObject expr);
    };

    struct FuncDecl{
        Token::Token type;
        Token::Token function;
        args_decl_t args;
        std::vector<StmtObject> functionBlock;
        FuncDecl(const Token::Token &type, const Token::Token &function, args_decl_t args, std::vector<StmtObject> functionBlock);
    };

    struct IfStmt{
        Expr::ExprObject condition;
        StmtObject thenBranch;
        std::optional<StmtObject> elseBranch;
        IfStmt(Expr::ExprObject condition, StmtObject thenBranch, std::optional<StmtObject> elseBranch);
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

    struct PrintStmt{
        Expr::ExprObject expr;
        explicit PrintStmt(Expr::ExprObject expr);
    };
}

namespace TinyC{
    class Parser{
    private:
        std::vector<Token::Token> tokens;
        std::vector<Token::Token>::const_iterator current;

        bool isAtEnd();
        bool check(Token::token_t type);
        Token::Token consume(Token::token_t type, std::string&& message);

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
        Expr::ExprObject CallExpr();
        Expr::ExprObject PrimaryExpr();

        Stmt::StmtObject VarDecl();
        Stmt::StmtObject FuncDecl();
        Stmt::StmtObject Stmt();
        Stmt::StmtObject IfStmt();
        Stmt::StmtObject WhileStmt();
        Stmt::StmtObject AssignStmt();
        Stmt::StmtObject ReturnStmt();
        Stmt::StmtObject PrintStmt();
        std::vector<Stmt::StmtObject> Block();
    public:
        explicit Parser(const std::string& source);
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
