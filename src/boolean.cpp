#include "../lib/boolean.h"

std::string BooleanExpression::format(){
	if (bool_operator == '>')
	{
		bool_operator = '<';
		NumericExpression* temp = left;
		left = right;
		right = temp;
		temp = nullptr;
	}

	return "[" + left->format() + " " + this->bool_operator + " " + right->format() + "]";
}

BooleanExpression::BooleanExpression(){

}

bool BooleanExpression::getValue(std::map<std::string, int>& var_map, std::map<std::string, std::map<int, int> >& arr_map){
	char bool_operator = this->bool_operator;

	if (bool_operator == '=')
	{
		if (this->left->getValue(var_map, arr_map) == this->right->getValue(var_map, arr_map)) return true;
		else return false;
	}
	else if (bool_operator == '<')
	{
		if (this->left->getValue(var_map, arr_map) < this->right->getValue(var_map, arr_map)) return true;
		else return false;
	}
	else if (bool_operator == '>')
	{
		if (this->left->getValue(var_map, arr_map) > this->right->getValue(var_map, arr_map)) return true;
		else return false;
	}
	else return false;
}

void BooleanExpression::setLeftNexp(NumericExpression* left){
	this->left = left;
	return;
}
NumericExpression* BooleanExpression::getLeftNexp(){
	return this->left;
}

void BooleanExpression::setRightNexp(NumericExpression* right){
	this->right = right;
	return;
}
NumericExpression* BooleanExpression::getRightNexp(){
	return this->right;
}

void BooleanExpression::setBoolOperator(char c){
	this->bool_operator = c;
	return;
}

BooleanExpression::~BooleanExpression(){
	delete this->left;
	delete this->right;
}