#include <fstream>
#include <sstream>
#include <iostream>
#include "Parse/Parser.hpp"
#include "Interpreter/Interpreter.hpp"
#include "AST/ASTDumper.hpp"
#include "boost/program_options.hpp"

namespace bpo = boost::program_options;
void parseCommandLineOption(const bpo::variables_map &vm, const bpo::options_description &description) {
    std::string filename;

    if(vm.count("help")) {
        std::cout << description << std::endl;
        return;
    }

    if(vm.count("file")) {
        filename = vm["file"].as<std::string>();
        tinyc::Parser parser(filename);
        if(vm.count("ast-dump")) {
            tinyc::ASTDumper dumper(parser);
            dumper.dump();
        }
        tinyc::Interpreter interpreter(parser);
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
            ("ast-dump", "Print nested AST");
    bpo::store(bpo::parse_command_line(argc, argv, description), vm);
    parseCommandLineOption(vm, description);
}
