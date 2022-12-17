#include <iostream>
#include <fstream>
#include <sstream>
#include "parser/scanner.hpp"
#include "parser/parser.hpp"
#include "ast/ast_dumper.hpp"

int main(int argc, const char* argv[]){
    // TODO: Support Command Line Option Parsing.
    //  Consider boost/program_option or llvm/Support/CommandLine.
    //  Compile command:
    //  -tokens-dump: print tokens.
    //  -ast-dump: print nested AST.

    std::string path = "test.txt";
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    const std::string source(buffer.str());
    TinyC::Token::Scanner scanner{source};
    auto tokens = scanner.scanTokens();
    TinyC::Parser parser{tokens};
    auto statements = parser.parse();
    TinyC::Stmt::DumpVisitor v{std::cout};
    for(auto &x: statements) std::visit(v, x);
    return 0;
}
