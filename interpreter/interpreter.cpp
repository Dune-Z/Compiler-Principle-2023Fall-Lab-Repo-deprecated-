#include "interpreter.hpp"

#include <utility>

namespace TinyC{
    Interpreter::Interpreter(const std::string &source) {
        Parser parser{source};
        statements = parser.parse();
        fvalue.resize(statements.size());
    }

    void Interpreter::interpret() {
        // Setting up initial environment.
        for(auto iter = statements.begin(); iter != statements.end(); iter++) {
            if(std::holds_alternative<std::unique_ptr<Stmt::FuncDecl>>(*iter)){
                auto &funcObject = std::get<std::unique_ptr<Stmt::FuncDecl>>(*iter);
                ftable.insert({funcObject->function.lexeme, iter - statements.begin()});
            } else {
                auto &varObject = std::get<std::unique_ptr<Stmt::VarDecl>>(*iter);
                Expr::EvaluateVisitor exprEvaluator{vtable};
                auto value = std::visit(exprEvaluator, varObject->expr);
                vtable.get()->define(varObject->variable, value, varObject->type.type);
            };
        }

        Stmt::EvaluateVisitor evaluator{vtable};
        working = ftable["main"];
        std::visit(evaluator, statements[working]);
    }

    void Interpreter::ast_dump() {
        Stmt::DumpVisitor dumper{std::cout};
        for(auto &statement: statements)
            std::visit(dumper, statement);
    }
}
