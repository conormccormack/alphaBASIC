#ifndef BOOLEAN_HPP
#define BOOLEAN_HPP

#include <string>
#include <exception>
#include <stdexcept>
#include <map>
#include "arithmetic.h"

/* Notes on design:
* This class probably could have been included with numeric and binary
* expressions in arithmetic.h, but I'm leaving it as as is to avoid 
* headaches. All boolean expressions contain a right and a left numeric
* expression as well as a boolean operator. 
*/

class BooleanExpression {
public:
	std::string format();
	BooleanExpression();
	~BooleanExpression(); 

	void setLeftNexp(NumericExpression* left);
	NumericExpression* getLeftNexp();

	void setRightNexp(NumericExpression* right);
	NumericExpression* getRightNexp();
	void setBoolOperator(char c);

	bool getValue(std::map<std::string, int>& var_map, std::map<std::string, std::map<int, int> >& arr_map);

private:
	NumericExpression* left;
	NumericExpression* right;
	char bool_operator;
};



#endif