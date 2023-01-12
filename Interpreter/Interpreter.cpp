#include "Interpreter.hpp"
#include "Environment.hpp"
namespace tinyc {
    Interpreter::Interpreter(Parser &parser) {
        stmts = parser.parse();
    }

    Interpreter::~Interpreter() {
        for(auto &stmt: stmts) delete stmt;
    }

    Literal Interpreter::interpret() {
        for(auto iter = stmts.begin(); iter != stmts.end(); iter++) {
            if((*iter)->nodeType() == VarDeclStmtNode) {
                auto varDeclNode = dynamic_cast<VarDeclStmt*>(*iter);
                auto valueInfo = new ValueInfo(varDeclNode->variable.first, evaluate(varDeclNode->expr));
                environment.setGlobalVar(varDeclNode->variable.second.lexeme, valueInfo);
            } else {
                auto funcDeclNode = dynamic_cast<FuncDeclStmt*>(*iter);
                auto valueInfo = new ValueInfo(
                        int(iter - stmts.begin()),
                        funcDeclNode->function.first,
                        std::nullopt
                        );
                auto paramTable = new ValueTable();
                for(auto &param: funcDeclNode->params) {
                    paramTable->insert({param.second.lexeme, ValueInfo(param.first, std::nullopt)});
                }
                environment.setGlobalFunc(funcDeclNode->function.second.lexeme, valueInfo, paramTable);
                if(funcDeclNode->function.second.lexeme == "main") return (*iter)->accept(*this);
            }
        }
        throw std::runtime_error("No function named 'main'.");
    }

    Literal Interpreter::evaluate(ASTNode *node) {
#define CAST_AND_DUMP(type) dynamic_cast<type*>(node)->accept(*this)
        switch (node->nodeType()) {
            case LiteralExprNode: return CAST_AND_DUMP(LiteralExpr);
            case VarExprNode: return CAST_AND_DUMP(VarExpr);
            case CallExprNode: return CAST_AND_DUMP(CallExpr);
            case UnaryExprNode: return CAST_AND_DUMP(UnaryExpr);
            case BinaryExprNode: return CAST_AND_DUMP(BinaryExpr);
            case GroupExprNode: return CAST_AND_DUMP(GroupExpr);
            case FuncDeclStmtNode: return CAST_AND_DUMP(FuncDeclStmt);
            case VarDeclStmtNode: return CAST_AND_DUMP(VarDeclStmt);
            case AssignStmtNode: return CAST_AND_DUMP(AssignStmt);
            case IfStmtNode: return CAST_AND_DUMP(IfStmt);
            case WhileStmtNode: return CAST_AND_DUMP(WhileStmt);
            case ReturnStmtNode: return CAST_AND_DUMP(ReturnStmt);
            case BlockStmtNode: return CAST_AND_DUMP(BlockStmt);
            case PrintStmtNode: return CAST_AND_DUMP(PrintStmt);
            default:
                throw std::runtime_error("Invalid AST Node in evaluation.");
        }
#undef CAST_AND_DUMP
    }
}
