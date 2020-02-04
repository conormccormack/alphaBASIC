#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <sstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stack>
#include "arithmetic.h"
#include "command.h"
#include "boolean.h"
#include <stdexcept>
#include <exception>


class Interpreter {
public:
    Interpreter(std::istream& in);
    ~Interpreter();

    void write(std::ostream& out);
    void write_from_map(std::ostream& out);

    void run_program();

    bool goSubStackIsEmpty();
    void printEmptyStackError(Command* currCommand);
    void printNonExistentLineError(Command* currCommand, std::string currCommandType, size_t jline);

    bool lineExists(size_t lineNumber);
    std::map<size_t, Command*>::iterator findJumpLine(size_t jline);

    Variable* parse_variable(std::stringstream& ss);
    NumericExpression* parse_numeric_expression(std::stringstream& ss);
    Constant* parse_constant(std::stringstream& ss);
    BooleanExpression* parse_boolean_expression(std::stringstream& ss);
    BinaryExpression* parse_binary_expression(std::stringstream& ss);

    std::map<std::string, int> var_map;
    // Map used for array type variables and their values
    std::map<std::string, std::map<int, int> > arr_map;

private:
    void parse(std::istream& in);
    // Map of BASIC line numbers to commands, which are used to execute program.
    std::map<size_t, Command*> command_map;
    // Stack of BASIC line numbers on which GOSUB commands occur.
    std::stack<size_t> gosub_stack;
    // Map used for storing basic variables and their values
};

#endif
