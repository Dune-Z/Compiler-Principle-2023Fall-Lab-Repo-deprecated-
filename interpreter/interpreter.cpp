#include "interpreter.hpp"

namespace TinyC{
    Interpreter::Interpreter(std::ostream &out, std::vector<Stmt::StmtObject> statements)
    : out{out}, statements{std::move(statements)} {}

    void Interpreter::interpret() {
        for(auto &statement: statements)
            std::visit(evaluator, statement);
    }
}
