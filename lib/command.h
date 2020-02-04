#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "arithmetic.h"
#include "boolean.h"
#include <string>
#include <sstream>
#include <exception>
#include <stdexcept>

/* COMMAND DESIGN
* In HW2, I elected to treat each line of our program as an object of type command.
* The command keyword at the start of each line determines the specific type of
* command that the objects takes on. This informs the functionality and datatypes
* that should be stored for each Command object. 
*/ 

class Command {
public:
    virtual std::string format() const = 0;
    virtual ~Command();
    std::string getCommandType();
    virtual size_t get_size_t_jline() const;
    virtual NumericExpression* getNexp() const;
    virtual BooleanExpression* getBexp() const;
    virtual Variable* getVariable() const;
    size_t getLineNumber();

protected: 
	std::size_t lineNumber;
	std::string command_type;
};


class PrintCommand : public	Command {
public:
	~PrintCommand();
	PrintCommand(std::size_t& line_number);
	void setNexp(NumericExpression* nexp);
	NumericExpression* getNexp() const;

	std::string format() const;

private:
	NumericExpression* nexp;
};


class LetVarCommand : public Command {
public:
	LetVarCommand(std::size_t& line_number);
	~LetVarCommand();

	void setVarName(Variable* varName);
	void setNexp(NumericExpression* nexp);
	NumericExpression* getNexp() const;

	Variable* getVariable() const;

	std::string format() const;

private:
	Variable* varName;
	NumericExpression* nexp;
};


class GotoCommand : public Command {
public:
	GotoCommand(std::size_t& line_number);
	~GotoCommand();

	void setJLine(Constant* jpointer);
	size_t get_size_t_jline() const;

	std::string format() const;

private:
	Constant* jline;
};


class IfCommand : public Command {
public:
	IfCommand(std::size_t& line_number);
	~IfCommand();

	std::string format() const;
	void setBooleanExpression(BooleanExpression* bexp);
	void setJLine(Constant* jpointer);
	int getJline();
	size_t get_size_t_jline() const;
	BooleanExpression* getBexp() const;


private:
	Constant* jline;
	BooleanExpression* bool_exp;
};


class GoSubCommand : public Command {
public:
	GoSubCommand(std::size_t& line_number);
	~GoSubCommand();

	void setJLine(Constant* jpointer);
	size_t get_size_t_jline() const;

	std::string format() const;

private:
	Constant* jline;
};


class ReturnCommand : public Command {
public:
	ReturnCommand(std::size_t& line_number);
	std::string format() const;
};


class EndCommand : public Command {
public:
	EndCommand(std::size_t& line_number);
	std::string format() const;
};


#endif