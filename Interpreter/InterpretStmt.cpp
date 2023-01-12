#include "Interpreter.hpp"
#include <iostream>

namespace tinyc {
    Literal Interpreter::visit(FuncDeclStmt* stmtNode) {
        Literal value = std::nullopt;
        for(auto &stmt: stmtNode->body) {
            value = evaluate(stmt);
            if(atReturn) {
                atReturn = false;
                return value;
            }
        }
        return value;
    }

    Literal Interpreter::visit(VarDeclStmt* stmtNode) {
        auto value = evaluate(stmtNode->expr);
        auto valueInfo = new ValueInfo(stmtNode->variable.first, value);
        environment.setClosureVar(stmtNode->variable.second.lexeme, valueInfo);
        return std::nullopt;
    }

    Literal Interpreter::visit(AssignStmt* stmtNode) {
        auto value = evaluate(stmtNode->expr);
        environment.setClosureVar(stmtNode->variable.lexeme, value);
        return std::nullopt;
    }

    Literal Interpreter::visit(IfStmt* stmtNode) {
        auto condition = evaluate(stmtNode->condition);
        if(std::get<int>(condition.value())) {
            return evaluate(stmtNode->thenBranch);
        } else if(stmtNode->elseBranch != nullptr) {
            return evaluate(stmtNode->elseBranch);
        }
        return std::nullopt;
    }

    Literal Interpreter::visit(WhileStmt* stmtNode) {
        Literal value = std::nullopt;
        while(std::get<int>(evaluate(stmtNode->condition).value())) {
            value = evaluate(stmtNode->body);
        }
        return value;
    }

    Literal Interpreter::visit(BlockStmt* stmtNode) {
        environment.enterClosure();
        Literal value = std::nullopt;
        for(auto &stmt: stmtNode->statements) {
            value = evaluate(stmt);
            if(atReturn) return value;
        }
        environment.exitClosure();
        return value;
    }

    Literal Interpreter::visit(ReturnStmt* stmtNode) {
        atReturn = true;
        return evaluate(stmtNode->expr);
    }

    template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
    template<class... Ts> overload(Ts...) -> overload<Ts...>;
    Literal Interpreter::visit(PrintStmt* stmtNode) {
        auto value = evaluate(stmtNode->expr);
        if(value.has_value()) {
            std::visit(overload{
                    [](auto arg){std::cout << arg << std::endl;}
            },value.value());

        } else throw std::runtime_error("Cannot print non-expression.");
        return std::nullopt;
    }
}