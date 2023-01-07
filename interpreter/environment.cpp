#include "Environment.hpp"

#include <utility>
namespace tinyc {
    void Closure::enterClosure() { localVarTable.push_back(new VarTable()); }
    void Closure::exitClosure() { localVarTable.pop_back(); }
    void Closure::set(const std::string &name, const Literal &literal) {
        for(auto it = localVarTable.rbegin(); it != localVarTable.rend(); it++) {
            auto &env = *it;
            auto found = env->find(name);
            if(found != env->end()) {
                found->second->literal= literal;
                return;
            }
        }
        throw std::runtime_error("Variable assign before declaration.");
    }

    void Closure::set(const std::string &name, TokenKind type, const Literal &literal) {
        if(localVarTable.empty()) {
            localVarTable.push_back(new VarTable());
        }
        auto env = *localVarTable.rbegin();
        auto found = env->find(name);
        if(found != env->end()) {
            throw std::runtime_error("Variable redefinition.");
        }
        auto valueInfo = new ValueInfo(type, literal);
        env->insert({name, valueInfo});
    }

    ValueInfo* Closure::get(const std::string &name) {
        for(auto it = localVarTable.rbegin(); it != localVarTable.rend(); it++) {
            auto &env = *it;
            auto found = env->find(name);
            if(found != env->end()) {
                return found->second;
            }
        }
        throw std::runtime_error("Variable access before declaration.");
    }

    void Environment::setGlobalVar(const std::string &name, const Literal &literal) {
        auto found = globalVarTable.find(name);
        if(found == globalVarTable.end()) {
            throw std::runtime_error("Variable assign before declaration.");
        }
        globalVarTable[name]->literal = literal;
    }

    void Environment::setGlobalVar(const std::string &name, TokenKind type, const Literal &literal) {
        auto found = globalVarTable.find(name);
        if(found != globalVarTable.end()) {
            throw std::runtime_error("Variable redefinition.");
        }
        auto valueInfo = new ValueInfo(type, literal);
        globalVarTable.insert({name, valueInfo});
    }

    void Environment::setGlobalFunc(Literal literal) {
        globalFuncTable[working]->literal = std::move(literal);
    }

    void Environment::setGlobalFunc(const std::string &name, int index, TokenKind type) {
        auto found = globalFuncTable.find(name);
        if(found != globalVarTable.end()) {
            throw std::runtime_error("Variable redefinition.");
        }
        auto valueInfo = new ValueInfo(index, type, std::nullopt);
        globalFuncTable.insert({name, valueInfo});
        closures.insert({name, new Closure()});
    }

    std::string Environment::switchEnv(std::string newWorking, bool initialize) {
        auto oldWorking = working;
        working = std::move(newWorking);
        if(initialize) {
            for(auto &varEntry: globalVarTable) {
                closures[working]->set(varEntry.first, varEntry.second->type, varEntry.second->literal);
            }
        }
        return oldWorking;
    }
}
