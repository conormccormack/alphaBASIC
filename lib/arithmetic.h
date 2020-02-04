#ifndef ARITHMETIC_HPP
#define ARITHMETIC_HPP

#include <string>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <map>

/* Notes on Design:
* Numerical expressions, binary expression, and variables, and
* the functionality of each thereof are outlined here. All of these are objects
* that will exclusively belong to objects of type command. All are dynanmically
* allocated, and numeric expression and binary expression are able to contain
* themselves (nested expressions).
*/

class NumericExpression {
public:
    virtual ~NumericExpression() {};
    virtual std::string format() const = 0;
    virtual int getValue(std::map<std::string, int>& var_map, std::map<std::string, std::map<int, int> >& arr_map) const = 0;
    virtual void printErrorMessage(std::map<std::string, int>& var_map, std::map<std::string, std::map<int, int> >& arr_map) const;
    virtual bool isArray() const;
    virtual NumericExpression* getArrIndex() const;
};


class Constant : public NumericExpression {
public:
	void setConstantValue(int val);
	int getConstantValue();
	std::string format() const;
	int getValue(std::map<std::string, int>& var_map, std::map<std::string, std::map<int, int> >& arr_map) const;
	size_t constant_to_size_t();

private:
	int value;
};

class BinaryExpression : public NumericExpression {
public:
	std::string format() const;
	BinaryExpression();
	~BinaryExpression();

	void setNexpLeft(NumericExpression* left);
	void setNexpRight(NumericExpression* right);
	void setExpOp(char c);
	int getValue(std::map<std::string, int>& var_map, std::map<std::string, std::map<int, int> >& arr_map) const;

	void printErrorMessage(std::map<std::string, int>& var_map, std::map<std::string, std::map<int, int> >& arr_map) const;

private:
	char bexp_operator;
	NumericExpression *left;
	NumericExpression *right;
};

class Variable : public NumericExpression {
public:
	Variable();
	~Variable();
	std::string& getVariableName();
	void setVariableName(std::string& varName);
	void setNexp(NumericExpression* nexp);
	int getValue(std::map<std::string, int>& var_map, std::map<std::string, std::map<int, int> >& arr_map) const;
	bool isArray() const;
	NumericExpression* getArrIndex() const;

	std::string format() const;

private:
	std::string varName;
	NumericExpression* nexp;
};

#endif
