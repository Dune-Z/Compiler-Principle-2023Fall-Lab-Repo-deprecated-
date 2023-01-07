#include "AST/AST.hpp"
#include "Interpreter.hpp"

namespace tinyc {
    Literal Interpreter::visit(LiteralExpr* exprNode) {
        if(!exprNode->literal.has_value()) {
            throw std::runtime_error("Literal doesn't hold value.");
        }
        return exprNode->literal;
    }

    Literal Interpreter::visit(VarExpr* exprNode) {
        auto valueInfo = environment.getClosure()->get(exprNode->variable);
        // TODO: Assert value matches its type.
        return valueInfo->literal;
    }

    Literal Interpreter::visit(CallExpr* exprNode) {
        std::vector<Literal> argsValue;
        for(auto &arg: exprNode->args) {
            argsValue.push_back(evaluate(arg));
        }
        auto fork = environment.switchEnv(exprNode->callee);
        environment.getClosure()->enterClosure();
        auto node = stmts[environment.getWorkingFunc()];
        auto function = dynamic_cast<FuncDeclStmt*>(node);
        if(function == nullptr) {
            throw std::runtime_error("No matching function call.");
        }
        auto params = function->params;
        for(int i = 0; i < exprNode->args.size(); i++) {
            environment.getClosure()->set(params[i].second, params[i].first, argsValue[i]);
        }
        visit(function);
        auto returnValue = environment.getReturnValue();
        environment.getClosure()->exitClosure();
        environment.switchEnv(fork, false);
        return returnValue;
    }

    Literal Interpreter::visit(UnaryExpr* exprNode) {
        UnaryOperationVisitor visitor(exprNode->op);
        auto value = evaluate(exprNode->rhs);
        return std::visit(visitor, value.value());
    }

    Literal Interpreter::visit(BinaryExpr* exprNode) {
        BinaryOperationVisitor visitor(exprNode->op);
        auto lvalue = evaluate(exprNode->lhs);
        auto rvalue = evaluate(exprNode->rhs);
        return std::visit(visitor, lvalue.value(), rvalue.value());
    }

    Literal Interpreter::visit(GroupExpr* exprNode) {
        return evaluate(exprNode->expr);
    }
}