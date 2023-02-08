#include "Lex/Lexer.hpp"
#include "Parse/Parser.hpp"
#include "AST/Dumper.hpp"
#include "boost/program_options.hpp"
#include <iostream>

using namespace tinyc;
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
    auto ast = parser.parse();
    if(vm.count("ast-dump")) {
      Dumper dumper(ast);
      dumper.dump();
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
          ("ast-dump", "Print nested AST");
  bpo::store(bpo::parse_command_line(argc, argv, description), vm);
  parseCommandLineOption(vm, description);
}

