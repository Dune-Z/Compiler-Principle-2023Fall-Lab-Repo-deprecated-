#include "Environment.hpp"
namespace tinyc {
    void Environment::enterClosure() {
        closures[working]->push_back(new ValueTable());
    }

    void Environment::exitClosure() {
        closures[working]->pop_back();
    }

    void Environment::setGlobalVar(const std::string &name, ValueInfo *valueInfo) {
        globalVarTable.insert({name, *valueInfo});
    }

    void Environment::setGlobalFunc(const std::string &name, ValueInfo *valueInfo, ValueTable* paramTable) {
        globalFuncTable.insert({name, *valueInfo});
        closures.insert({name, new std::vector<ValueTable*>()});
        closures[name]->push_back(paramTable);
    }

    void Environment::setClosureVar(const std::string &name, ValueInfo *valueInfo) {
//        for(auto iter = closures[working]->rbegin(); iter != closures[working]->rend(); iter++) {
//            auto found = (*iter)->find(name);
//            if(found != (*iter)->end()) {
//                throw std::runtime_error("Variable redefinition.");
//            }
//        }
//        if(globalVarTable.find(name) != globalVarTable.end())
//            throw std::runtime_error("Variable redefinition.");
        auto env = *closures[working]->rbegin();
        env->insert({name, *valueInfo});
    }

    void Environment::setClosureVar(const std::string &name, const Literal &literal) {
        for(auto iter = closures[working]->rbegin(); iter != closures[working]->rend(); iter++) {
            auto found = (*iter)->find(name);
            if(found != (*iter)->end()) {
                found->second.literal = literal;
                return;
            }
        }
        auto global = globalVarTable.find(name);
        if(global == globalVarTable.end()) {
            throw std::runtime_error("Variable assign before definition.");
        } else {
            global->second.literal = literal;
        }
    }

    int Environment::getGlobalFunc(const std::string &name) {
        return globalFuncTable[name].function;
    }

    Literal Environment::getClosureVar(const std::string &name) {
        for(auto iter = closures[working]->rbegin(); iter != closures[working]->rend(); iter++) {
            auto found = (*iter)->find(name);
            if(found != (*iter)->end()) {
                return found->second.literal;
            }
        }
        auto global = globalVarTable.find(name);
        if(global == globalVarTable.end()) {
            throw std::runtime_error("Variable access before definition.");
        } else {
            return global->second.literal;
        }
    }
}
