#include "interpreter.hpp"
#include "parser/parser.hpp"

namespace TinyC{
    Interpreter::Interpreter(std::ostream &out, const std::string &source)
    : out{out} {
        Parser parser{source};
        statements = parser.parse();
    }

    void Interpreter::interpret() {
        for(auto &statement: statements)
            std::visit(evaluator, statement);
        evaluator.dump_table();
    }

    void Interpreter::ast_dump() {
        for(auto &statement: statements)
            std::visit(dumper, statement);
    }
}
