#include "environment.hpp"
#include <unordered_map>
#include <iostream>
#include <utility>

namespace TinyC{
    Environment::Environment(EnvObject parent): parent{std::move(parent)}, table{} {}

    void Environment::define(const Token::Token &token, const Token::literal_t &literal, const Token::token_t &type) {
        table.insert({token.lexeme, Value{type, literal}});
    }

    void Environment::set(const Token::Token &token, const Token::literal_t &literal) {
        auto found = table.find(token.lexeme);
        if(found == table.end()) {
            if(parent) {
                parent->set(token, literal);
                return;
            }
            else throw std::overflow_error("Assign before declaration.");
            // TODO: Handling Error.
        }
        table[token.lexeme].value = literal;
    }

    Token::literal_t Environment::get(const Token::Token &token) {
        auto found = table.find(token.lexeme);
        if(found == table.end()) {
            if(parent) return parent->get(token);
            else throw std::overflow_error("Assign before declaration.");
            // TODO: Handling Error.
        }
        return table[token.lexeme].value;
    }

    void Environment::dump_table() {
        for(auto &x: table){
            std::cout << "Variable: " << x.first << "; Type: " << x.second.type << "; Value: ";
            if(x.second.type == TinyC::Token::TOKEN_TYPE_STRING)
                std::cout << std::get<std::string>(x.second.value.value());
            if(x.second.type == TinyC::Token::TOKEN_TYPE_INT)
                std::cout << std::get<int>(x.second.value.value());
            if(x.second.type == TinyC::Token::TOKEN_TYPE_BOOLEAN)
                std::cout << bool(std::get<int>(x.second.value.value()));
            if(x.second.type == TinyC::Token::TOKEN_TYPE_FLOAT){
                if(std::holds_alternative<int>(x.second.value.value()))
                    std::cout << std::get<int>(x.second.value.value());
                else std::cout << std::get<double>(x.second.value.value());
            }
            std::cout << std::endl;
        }
        parent->dump_table();
    }
}