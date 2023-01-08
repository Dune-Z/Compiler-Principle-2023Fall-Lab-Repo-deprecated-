#include "Interpreter.hpp"
#include "Environment.hpp"
namespace tinyc {
    Interpreter::Interpreter(Parser &parser) {
        stmts = parser.parse();
    }

    Literal Interpreter::interpret() {
        for(auto iter = stmts.begin(); iter != stmts.end(); iter++) {
            if((*iter)->nodeType() == VarDeclStmtNode) {
                auto varDeclNode = dynamic_cast<VarDeclStmt*>(*iter);
                auto valueInfo = new ValueInfo(varDeclNode->variable.first, evaluate(varDeclNode->expr));
                environment.setGlobalVar(varDeclNode->variable.second, valueInfo);
            } else {
                auto funcDeclNode = dynamic_cast<FuncDeclStmt*>(*iter);
                auto valueInfo = new ValueInfo(
                        int(iter - stmts.begin()),
                        funcDeclNode->function.first,
                        std::nullopt
                        );
                auto paramTable = new ValueTable();
                for(auto &param: funcDeclNode->params) {
                    paramTable->insert({param.second, ValueInfo(param.first, std::nullopt)});
                }
                environment.setGlobalFunc(funcDeclNode->function.second, valueInfo, paramTable);
                if(funcDeclNode->function.second == "main") return (*iter)->accept(*this);
            }
        }
        throw std::runtime_error("No function named 'main'.");
    }

    Literal Interpreter::evaluate(ASTNode *node) {
#define CAST_ABD_EVALUATE(type) dynamic_cast<type*>(node)->accept(*this)
        switch (node->nodeType()) {
            case LiteralExprNode: return CAST_ABD_EVALUATE(LiteralExpr);
            case VarExprNode: return CAST_ABD_EVALUATE(VarExpr);
            case CallExprNode: return CAST_ABD_EVALUATE(CallExpr);
            case UnaryExprNode: return CAST_ABD_EVALUATE(UnaryExpr);
            case BinaryExprNode: return CAST_ABD_EVALUATE(BinaryExpr);
            case GroupExprNode: return CAST_ABD_EVALUATE(GroupExpr);
            case FuncDeclStmtNode: return CAST_ABD_EVALUATE(FuncDeclStmt);
            case VarDeclStmtNode: return CAST_ABD_EVALUATE(VarDeclStmt);
            case AssignStmtNode: return CAST_ABD_EVALUATE(AssignStmt);
            case IfStmtNode: return CAST_ABD_EVALUATE(IfStmt);
            case WhileStmtNode: return CAST_ABD_EVALUATE(WhileStmt);
            case ReturnStmtNode: return CAST_ABD_EVALUATE(ReturnStmt);
            case BlockStmtNode: return CAST_ABD_EVALUATE(BlockStmt);
            case PrintStmtNode: return CAST_ABD_EVALUATE(PrintStmt);
            default:
                throw std::runtime_error("Invalid AST Node in evaluation.");
        }
#undef CAST_ABD_EVALUATE
    }
}
