#include "../lib/arithmetic.h"
#include <iostream>

bool NumericExpression::isArray() const {
	return false;
}

void NumericExpression::printErrorMessage(std::map<std::string, int>& var_map, std::map<std::string, std::map<int, int> >& arr_map) const {
	return;
}

NumericExpression* NumericExpression::getArrIndex() const {
	return nullptr;
}

// All CONSTANT functions
// Sets int value at constant
void Constant::setConstantValue(int val){
	this->value = val;
	return;
}


// Returns string of constant for pretty printing
std::string Constant::format() const {
	std::stringstream ss;
	std::string s;
	ss << this->value;
	ss >> s;
	return s;
}

size_t Constant::constant_to_size_t()
{
	std::stringstream ss;
	size_t to_return;
	ss << this->value;
	ss >> to_return;
	return to_return;
}

// getter for Constant's value
int Constant::getConstantValue(){
	return this->value;
}

// Same as above.
int Constant::getValue(std::map<std::string, int>& var_map, std::map<std::string, std::map<int, int> >& arr_map) const{
	return this->value;
}

// All BINARYEXPRESSION functions
BinaryExpression::BinaryExpression(){ }


void BinaryExpression::printErrorMessage(std::map<std::string, int>& var_map, std::map<std::string, std::map<int, int> >& arr_map) const{
	std::cout << "Division by 0: " << this->left->format() << " = " << this->left->getValue(var_map, arr_map) << ", " << this->right->format() << " = " << this->right->getValue(var_map, arr_map) << "." << std::endl;
	return;
}

int	BinaryExpression::getValue(std::map<std::string,int>& var_map, std::map<std::string, std::map<int, int> >& arr_map) const{
	char bexp_operator = this->bexp_operator;

	if (bexp_operator == '+') return this->left->getValue(var_map, arr_map) + this->right->getValue(var_map, arr_map);
	else if (bexp_operator == '-') return this->left->getValue(var_map, arr_map) - this->right->getValue(var_map, arr_map);
	else if (bexp_operator == '*') return this->left->getValue(var_map, arr_map) * this->right->getValue(var_map, arr_map);
	else if (bexp_operator == '/'){
		if (this->right->getValue(var_map, arr_map) == 0) throw std::logic_error("Div by zero");
		else return this->left->getValue(var_map, arr_map) / this->right->getValue(var_map, arr_map);
	}

	return 0;
}


BinaryExpression::~BinaryExpression() {
    delete this->left;
    delete this->right;
}

// Setter for left numeric expression
void BinaryExpression::setNexpLeft(NumericExpression* left){
	this->left = left;
}

// Setter for right numeric expression
void BinaryExpression::setNexpRight(NumericExpression* right){
	this->right = right;
}

// Setter for binary operator
void BinaryExpression::setExpOp(char c){
	this->bexp_operator = c;
}

// Returns string for pretty printing
std::string BinaryExpression::format() const {
    return "(" + this->left->format() + ' ' + this->bexp_operator + ' ' + this->right->format() + ")";
}


// All VARIABLE functions
// Setter for varName
void Variable::setVariableName(std::string& varName){
	this->varName = varName;
	return;
}

/// Getter for varName
std::string& Variable::getVariableName(){
	return this->varName;
}

Variable::Variable(){
	nexp = nullptr;
}

NumericExpression* Variable::getArrIndex() const{
	return this->nexp;
}

bool Variable::isArray() const {
	if (nexp) return true;
	else return false;
}

// Returns string for pretty printing
std::string Variable::format() const {
	if (nexp == nullptr) return varName;
	else return varName + '[' + this->nexp->format() + ']';
}

void Variable::setNexp(NumericExpression* nexp){
	this->nexp = nexp;
	return;
}

Variable::~Variable(){
	if (nexp) delete this->nexp;
}

// Returns in value of simple var type given name
int Variable::getValue(std::map<std::string, int>& var_map, std::map<std::string, std::map<int, int> >& arr_map) const{
	if (isArray()){
		std::map<std::string, std::map<int, int> >::iterator outer_itr;
		std::map<int, int>::iterator inner_itr;
		outer_itr = arr_map.find(this->varName);
		inner_itr = outer_itr->second.find(this->nexp->getValue(var_map, arr_map));
		return inner_itr->second;
	}
	else {
		std::map<std::string, int>::iterator map_itr;
		map_itr = var_map.find(this->varName);
		return map_itr->second;
	}
	return 0;
}
