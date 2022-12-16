#include <iostream>
#include <fstream>
#include <sstream>
#include "parser/scanner.hpp"
#include "parser/parser.hpp"
#include "parser/ast.hpp"

int main(int argc, const char* argv[]){
    // TODO: Support Command Line Option Parsing. Consider boost/program_option or llvm/Support/CommandLine
    std::string path = "test.txt";
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    const std::string source(buffer.str());
    TinyC::Token::Scanner scanner{source};
    TinyC::Parser parser{scanner.scanTokens()};
    auto statements = parser.parse();
    return 0;
}
