#ifndef INTERPRETER_INTERPRETER_HPP
#define INTERPRETER_INTERPRETER_HPP
#include <ostream>

namespace TinyC{
    class Interpreter{
    private:
        std::ostream &out;
    public:
        Interpreter(std::ostream &out);
    };
}

#endif //INTERPRETER_INTERPRETER_HPP
