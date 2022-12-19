#include <iostream>
#include <fstream>
#include "boost/program_options.hpp"
#include "parser/scanner.hpp"
#include "parser/parser.hpp"
#include "ast/ast_dumper.hpp"
#include "ast/ast_evaluator.hpp"

int main(int argc, const char* argv[]){
    boost::program_options::options_description options("");
    boost::program_options::variables_map vm;
    options.add_options()
            ("help, h", "Help Screen")
            ("file, f", boost::program_options::value<std::string>(), "Usage: Interpreter --filename [filename]")
            ("interpret", "interpret")
            ("ast-dump", "print nested AST");
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, options), vm);
    if(vm.count("help")){
        std::cout << options << std::endl;
        return 0;
    }

    std::string path;
    if(vm.count("file")){
        path = vm["file"].as<std::string>();
        std::ifstream file(path);
        std::stringstream buffer;
        buffer << file.rdbuf();
        const std::string source(buffer.str());
        TinyC::Token::Scanner scanner{source};
        auto tokens = scanner.scanTokens();
        TinyC::Parser parser{tokens};
        auto statements = parser.parse();
        if(vm.count("ast-dump")){
            TinyC::Stmt::DumpVisitor v{std::cout};
            for(auto &x: statements) std::visit(v, x);
        }
        if(vm.count("interpret")) {
            TinyC::Stmt::EvaluateVisitor v;
            for(auto &x: statements) std::visit(v, x);
            auto table = v.table;
            for(auto &x: table){
                std::cout << "Variable: " << x.first << "; Type: " << x.second.second << "; Value: ";
                if(x.second.second == TinyC::Token::TOKEN_TYPE_STRING)
                    std::cout << std::get<std::string>(x.second.first.value());
                if(x.second.second == TinyC::Token::TOKEN_TYPE_INT)
                    std::cout << std::get<int>(x.second.first.value());
                if(x.second.second == TinyC::Token::TOKEN_TYPE_BOOLEAN)
                    std::cout << std::get<bool>(x.second.first.value());
                if(x.second.second == TinyC::Token::TOKEN_TYPE_FLOAT)
                    std::cout << std::get<double>(x.second.first.value());
                std::cout << std::endl;
            }
        }
        return 0;
    } else {

    }
}
