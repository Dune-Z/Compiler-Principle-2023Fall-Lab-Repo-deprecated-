#include "AST/ASTVisitor.hpp"
#include "Interpreter.hpp"

namespace tinyc {

    Literal Interpreter::visit(FuncDeclStmt* stmtNode){
        for(auto &stmt: stmtNode->body) {
            if(stmt->nodeType() == ReturnStmtNode) {
                
            }
            evaluate(stmt);
        }
        return std::nullopt;
    }

    Literal Interpreter::visit(VarDeclStmt* stmtNode){
        Literal value = std::nullopt;
        if(stmtNode->expr) value = evaluate(stmtNode->expr);
        environment.getClosure()->set(stmtNode->variable.second, stmtNode->variable.first, value);
        return std::nullopt;
    }

    Literal Interpreter::visit(AssignStmt* stmtNode){
        environment.getClosure()->set(stmtNode->variable, evaluate(stmtNode->expr));
        return std::nullopt;
    }

    Literal Interpreter::visit(IfStmt* stmtNode){
        auto flag = evaluate(stmtNode->condition);
        if(std::get<int>(flag.value())) evaluate(stmtNode->thenBranch);
        else if(stmtNode->elseBranch) evaluate(stmtNode->elseBranch);
        return std::nullopt;
    }

    Literal Interpreter::visit(WhileStmt* stmtNode){
        while(evaluate(stmtNode->condition)) {
            evaluate(stmtNode->body);
        }
        return std::nullopt;
    }

    Literal Interpreter::visit(BlockStmt* stmtNode){
        environment.getClosure()->enterClosure();
        for(auto &stmt: stmtNode->statements)
            evaluate(stmt);
        environment.getClosure()->exitClosure();
        return std::nullopt;
    }

    Literal Interpreter::visit(ReturnStmt* stmtNode){
        environment.setGlobalFunc(evaluate(stmtNode->expr));
        return std::nullopt;
    }

    template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
    template<class... Ts> overload(Ts...) -> overload<Ts...>;
    Literal Interpreter::visit(PrintStmt* stmtNode){
        std::visit(overload{
                [](auto arg){std::cout << arg << std::endl;}
        }, evaluate(stmtNode->expr).value());
        return std::nullopt;
    }
}