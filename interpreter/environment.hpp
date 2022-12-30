#ifndef INTERPRETER_ENVIRONMENT_HPP
#define INTERPRETER_ENVIRONMENT_HPP
#include "parser/token.hpp"
#include <unordered_map>

namespace TinyC{
    class Environment;
    struct Value;
    using EnvObject = std::shared_ptr<Environment>;
    using table_t = std::unordered_map<std::string, Value>;

    struct Value{
        Token::token_t type;
        Token::literal_t value;
    };

    class Environment{
    private:
        EnvObject parent;
        table_t table;
    public:
        Environment() = default;
        explicit Environment(EnvObject parent);
        void define(const Token::Token &token, const Token::literal_t &literal, const Token::token_t &type);
        void set(const Token::Token &token, const Token::literal_t &literal);
        Token::literal_t get(const Token::Token &token);
        void dump_table();
    };
}


#endif //INTERPRETER_ENVIRONMENT_HPP
