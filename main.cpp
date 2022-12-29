#include <iostream>
#include <fstream>
#include "boost/program_options.hpp"
#include "parser/scanner.hpp"
#include "parser/parser.hpp"
#include "parser/dumper.hpp"
#include "interpreter/evaluator.hpp"
namespace bpo = boost::program_options;

void parseCommandLineOption(const bpo::variables_map &vm, const bpo::options_description &description) {
    std::string filename;

    if(vm.count("help")) {
        std::cout << description << std::endl;
        return;
    }

    if(vm.count("file")) {
        filename = vm["file"].as<std::string>();
        std::ifstream file(filename);
        std::stringstream buffer;
        buffer << file.rdbuf();
        const std::string source(buffer.str());
        TinyC::Token::Scanner scanner{source};
        auto tokens = scanner.scanTokens();
        TinyC::Parser parser{tokens};
        auto statements = parser.parse();

        if(vm.count("ast-dump")) {
            TinyC::Stmt::DumpVisitor v{std::cout};
            for(auto &x: statements) std::visit(v, x);
        }
        if(vm.count("interpret")) {
            TinyC::Stmt::EvaluateVisitor v;
            for(auto &x: statements) std::visit(v, x);
            for(auto &x: v.table){
                std::cout << "Variable: " << x.first << "; Type: " << x.second.second << "; Value: ";
                if(x.second.second == TinyC::Token::TOKEN_TYPE_STRING)
                    std::cout << std::get<std::string>(x.second.first.value());
                if(x.second.second == TinyC::Token::TOKEN_TYPE_INT)
                    std::cout << std::get<int>(x.second.first.value());
                if(x.second.second == TinyC::Token::TOKEN_TYPE_BOOLEAN)
                    std::cout << bool(std::get<int>(x.second.first.value()));
                if(x.second.second == TinyC::Token::TOKEN_TYPE_FLOAT){
                    if(std::holds_alternative<int>(x.second.first.value()))
                        std::cout << std::get<int>(x.second.first.value());
                    else std::cout << std::get<double>(x.second.first.value());
                }
                std::cout << std::endl;
            }
        }

    } else {
        std::cerr << "Expected filename. Usage: --file <filename>" <<  std::endl;
        return;
    }
}

int main(int argc, const char* argv[]){
    bpo::options_description description("");
    bpo::variables_map vm;
    description.add_options()
            ("help, h", "Help Screen")
            ("file, f", bpo::value<std::string>(), "filename")
            ("interpret", "call for interpreter")
            ("ast-dump", "print nested AST");
    bpo::store(bpo::parse_command_line(argc, argv, description), vm);
    parseCommandLineOption(vm, description);
}
