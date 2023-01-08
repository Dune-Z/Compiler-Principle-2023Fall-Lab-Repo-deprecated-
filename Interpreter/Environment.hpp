#ifndef TINYC_ENVIRONMENT_HPP
#define TINYC_ENVIRONMENT_HPP
#include "Lex/Token.hpp"
#include <vector>
#include <unordered_map>

namespace tinyc {
    struct ValueInfo {
        int function = 0;
        TokenKind type = TOKEN_EOF;
        Literal literal;

        ValueInfo() = default;
        ValueInfo(TokenKind type, Literal literal)
        : type(type), literal(std::move(literal)) {}
        ValueInfo(int function, TokenKind type, Literal literal)
        : function(function), type(type), literal(std::move(literal)) {}
    };

    using ValueTable = std::unordered_map<std::string, ValueInfo>;
    using Closure = std::vector<ValueTable*>;
    class Environment {
    private:
        ValueTable globalVarTable;
        ValueTable globalFuncTable;
        std::unordered_map<std::string, Closure*> closures;
    public:
        std::string working = "main";
    public:
        Environment() = default;
        ~Environment();
        void enterClosure();
        void exitClosure();
        void setGlobalVar(const std::string &name, ValueInfo* valueInfo);
        void setGlobalFunc(const std::string &name, ValueInfo* valueInfo, ValueTable* paramTable);
        void setClosureVar(const std::string &name, ValueInfo* valueInfo);
        void setClosureVar(const std::string &name, const Literal &literal);
        int getGlobalFunc(const std::string &name);
        Literal getClosureVar(const std::string &name);
    };
}





#endif //TINYC_ENVIRONMENT_HPP
