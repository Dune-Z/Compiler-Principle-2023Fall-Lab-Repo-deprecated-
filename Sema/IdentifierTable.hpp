#ifndef TINYC_IDENTIFIERTABLE_HPP
#define TINYC_IDENTIFIERTABLE_HPP
#include "Lex/Token.hpp"
#include "AST/AST.hpp"
#include <unordered_map>
namespace tinyc {
// IdentifierInfo contains necessary information that is utilized in symbol table.
struct IdentifierInfo {
    bool isUsed = false;
    Stmt *decl = nullptr;
    TokenKind type = TOKEN_EOF;
    IdentifierInfo(Stmt *decl, TokenKind type)
    : decl(decl), type(type) {}
};

using IdentifierTable = std::unordered_map<std::string, IdentifierInfo>;
}

#endif //TINYC_IDENTIFIERTABLE_HPP
