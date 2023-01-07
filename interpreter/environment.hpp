#ifndef TINYC_ENVIRONMENT_HPP
#define TINYC_ENVIRONMENT_HPP
#include "Lex/Token.hpp"
#include "AST/AST.hpp"
#include <unordered_map>
#include <vector>

namespace tinyc{
    struct ValueInfo {
        int funcIndex = 0;
        TokenKind type;
        Literal literal;
        ValueInfo(TokenKind type, Literal literal)
                : type(type), literal(std::move(literal)) {}
        ValueInfo(int funcIndex, TokenKind type, Literal literal)
                : funcIndex(funcIndex), type(type), literal(std::move(literal)) {}
    };

    using VarTable = std::unordered_map<std::string, ValueInfo*>;
    using FuncTable = std::unordered_map<std::string, ValueInfo*>;
    class Closure {
    private:
        std::vector<VarTable*> localVarTable;
    public:
        Closure() = default;
        void enterClosure();
        void exitClosure();
        void set(const std::string &name, const Literal &literal);
        void set(const std::string &name, TokenKind type, const Literal &literal);
        ValueInfo *get(const std::string &name);
    };

    class Environment {
    private:
        std::string working = "main";
        VarTable globalVarTable;
        FuncTable globalFuncTable;
        std::unordered_map<std::string, Closure*> closures;
    public:
        Environment() = default;
        void setGlobalVar(const std::string &name, const Literal &literal);
        void setGlobalVar(const std::string &name, TokenKind type, const Literal &literal);
        void setGlobalFunc(Literal literal);
        void setGlobalFunc(const std::string &name, int index, TokenKind type);
        std::string switchEnv(std::string newWorking, bool initialize = true);
        Closure* getClosure() {return closures[working];}
        int getWorkingFunc() {return globalFuncTable[working]->funcIndex;}
        Literal getReturnValue() {return globalFuncTable[working]->literal;}
    };
}

#endif //TINYC_ENVIRONMENT_HPP
