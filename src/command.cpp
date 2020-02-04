#include "../lib/command.h"
#include <iostream>

Command::~Command(){}

std::string Command::getCommandType(){
	return this->command_type;
}

size_t Command::getLineNumber(){
	return this->lineNumber;
}

Variable* Command::getVariable() const { return nullptr; }

size_t Command::get_size_t_jline() const {return 0; }

NumericExpression* Command::getNexp() const{ return nullptr; }

BooleanExpression* Command::getBexp() const {return nullptr; }

//All Print Command functions
PrintCommand::PrintCommand(size_t &line_number){
	this->lineNumber = line_number;
	this->command_type = "PRINT";
}

PrintCommand::~PrintCommand(){
	delete this->nexp;
}

std::string PrintCommand::format() const{
	std::stringstream ss;
	std::string s;
	ss << this->lineNumber;
	ss >> s;
	return s + " PRINT " + nexp->format();
}

void PrintCommand::setNexp(NumericExpression* nexp){
	this->nexp = nexp;
	return;
}

NumericExpression* PrintCommand::getNexp() const{
	return this->nexp;
}


//All LetVarCommand functions
LetVarCommand::LetVarCommand(std::size_t& line_number){
	this->lineNumber = line_number;
	this->command_type = "LET";
}


Variable* LetVarCommand::getVariable() const {
	return this->varName;
}


LetVarCommand::~LetVarCommand(){
	delete this->varName;
	delete this->nexp;
}


void LetVarCommand::setVarName(Variable* varName){
	this->varName = varName;
}

void LetVarCommand::setNexp(NumericExpression* nexp){
	this->nexp = nexp;
}

std::string LetVarCommand::format() const{
	std::stringstream ss;
	std::string s;
	ss << this->lineNumber;
	ss >> s;
	std::cout << this->varName->format() << std::endl;
	return s + " LET " + this->varName->format() + " " + this->nexp->format();
}

NumericExpression* LetVarCommand::getNexp() const {
	return this->nexp;
}

//All GotoCommand functions
GotoCommand::GotoCommand(std::size_t& line_number)
{
	this->lineNumber = line_number;
	this->command_type = "GOTO";
}

GotoCommand::~GotoCommand(){
	delete this->jline;
}

void GotoCommand::setJLine(Constant* jpointer){
	this->jline = jpointer;
	return;
}

size_t GotoCommand::get_size_t_jline() const{
	return this->jline->constant_to_size_t();
}


std::string GotoCommand::format() const{
	std::stringstream ss;
	std::string s;
	std::string jline;
	int j;
	ss << this->lineNumber;
	ss >> s;
	j = this->jline->getConstantValue();
	jline = std::to_string(j);
	return s + " GOTO " + "<" + jline + ">";
}


//All IfComand functions
IfCommand::IfCommand(std::size_t& line_number){
	this->lineNumber = line_number;
	this->command_type = "IF";
}

IfCommand::~IfCommand(){
	delete this->bool_exp;
	delete this->jline;
}

BooleanExpression* IfCommand::getBexp() const {
	return this->bool_exp;
}

void IfCommand::setBooleanExpression(BooleanExpression* bexp)
{
	this->bool_exp = bexp;
	return;
}

void IfCommand::setJLine(Constant* jpointer)
{
	this->jline = jpointer;
	return;
}

int IfCommand::getJline(){
	return this->jline->getConstantValue();
}

size_t IfCommand::get_size_t_jline() const{
	return this->jline->constant_to_size_t();
}

std::string IfCommand::format() const{
	std::stringstream ss;
	std::string s;
	ss << this->lineNumber;
	ss >> s;
	int j = this->jline->getConstantValue();
	std::string jline;
	jline = std::to_string(j);

	return s + " IF " + bool_exp->format() + " THEN " + "<" + jline + ">";
}

//All GoSubCommand functions
GoSubCommand::GoSubCommand(std::size_t& line_number){
	this->lineNumber = line_number;
	this->command_type = "GOSUB";
}

GoSubCommand::~GoSubCommand(){
	delete this->jline;
}

void GoSubCommand::setJLine(Constant* jpointer){
	this->jline = jpointer;
	return;
}

size_t GoSubCommand::get_size_t_jline() const{
	return this->jline->constant_to_size_t();
}

std::string GoSubCommand::format() const
{
	std::stringstream ss;
	std::string s;
	std::string jline;
	int j;
	ss << this->lineNumber;
	ss >> s;
	j = this->jline->getConstantValue();
	jline = std::to_string(j);
	return s + " GOSUB " + "<" + jline + ">";
}

//All ReturnCommand functions
ReturnCommand::ReturnCommand(std::size_t& line_number){
	this->lineNumber = line_number;
	this->command_type = "RETURN";
}

std::string ReturnCommand::format() const{
	std::stringstream ss;
	std::string s;
	ss << this->lineNumber;
	ss >> s;
	return s + " RETURN";
}


//All EndCommand functions
EndCommand::EndCommand(std::size_t& line_number)
{
	this->lineNumber = line_number;
	this->command_type = "END";
}

std::string EndCommand::format() const{
	std::stringstream ss;
	std::string s;
	ss << this->lineNumber;
	ss >> s;
	return s + " END";
}