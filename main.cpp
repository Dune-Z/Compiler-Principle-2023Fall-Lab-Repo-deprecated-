#include <iostream>
#include <fstream>
#include "boost/program_options.hpp"
#include "parser/scanner.hpp"
#include "interpreter/interpreter.hpp"
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
        if(source.empty()) {
            std::cerr << "No such file or empty file: " << filename << std::endl;
            exit(42);
        }
        TinyC::Interpreter interpreter{source};

        if(vm.count("ast-dump")) 
            interpreter.ast_dump();
        if(vm.count("interpret")) 
            interpreter.interpret();

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
