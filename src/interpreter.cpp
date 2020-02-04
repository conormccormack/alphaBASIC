#include "../lib/interpreter.h"
#include <iostream>

Interpreter::Interpreter(std::istream& in) {
    this->parse(in);
}

Interpreter::~Interpreter() {   
    // Deallocates all commands from heap using dynamic binding
    Command* to_delete;
    std::map<size_t, Command*>::iterator map_itr;
    for (map_itr = command_map.begin(); map_itr != command_map.end(); map_itr++)
    {
        to_delete = map_itr->second;
        delete to_delete;
    }

    to_delete = nullptr;
}

// Iterate through command map, execute appropriate actions
void Interpreter::run_program(){
    std::map<size_t, Command*>::iterator map_itr;

    /* Note that map_itr increment does not occur in for loop.
    * This is to accomodate line jumps to the first line of the program.
    * This requires map_itr to be incremented at end of loop, and before each break keyword.*/
    for (map_itr = command_map.begin(); map_itr != command_map.end();   )
    {
        // Retrieve command pointer and type from map
        Command* currCommand = map_itr->second;
        std::string currCommandType = currCommand->getCommandType();

        try {
            // Execute appropriate commands
            if (currCommandType == "PRINT"){
                // Retrieve nexp, retrive  value of nexp, print to command line.
                NumericExpression* nexp = currCommand->getNexp();
                std::cout << nexp->getValue(var_map, arr_map) << std::endl;
            }
            else if (currCommandType == "IF"){
                size_t jline = currCommand->get_size_t_jline();
                if (!lineExists(jline)){
                    printNonExistentLineError(currCommand, currCommandType, jline);
                    map_itr++;
                    break;
                } 
                else {
                    // Determine boolean value of bexp, go to jumpline if true.
                    // Do nothing if bexp evaluates to false.
                    BooleanExpression* bexp = currCommand->getBexp();
                    if (bexp->getValue(var_map, arr_map)) {
                        map_itr = command_map.find(jline);
                        continue;
                    }
                    else { // *insert nothing here*
                    }
                }
            }
            else if (currCommandType == "LET"){
                // Retrieve pointers to nexp (variable value) and var (variable/array name & index)
                NumericExpression* nexp = currCommand->getNexp();
                Variable* var = currCommand->getVariable();
                std::string varName = var->getVariableName();
                int assignment_val = nexp->getValue(var_map, arr_map);
                // If variable is array, find entry at array index, assign/overwrite
                if (var->isArray()){
                    std::map<std::string, std::map<int, int> >::iterator outer_itr;
                    outer_itr = arr_map.find(varName);
                    NumericExpression* arr_index_ptr = var->getArrIndex();
                    int index = arr_index_ptr->getValue(var_map, arr_map);
                    // If array does not exist yet, add new string, map pair to arr_map
                    if (outer_itr == arr_map.end()) {
                        std::map<int, int> inner;
                        inner.insert(std::pair<int, int>(index , assignment_val));
                        arr_map.insert(std::pair<std::string, std::map<int, int> >(varName, inner));
                    }
                    // If array exists, overwrite/assign at index
                    else {
                        // use temp val to overwrite value at array index
                        std::map<int, int>::iterator inner_itr;
                        inner_itr = outer_itr->second.find(index);
                        // If inner_itr == end(), then index has not been initilaized
                        // So add index and assignment val to map
                        if (inner_itr == outer_itr->second.end()){
                            outer_itr->second.insert(std::pair<int, int>(index, assignment_val));
                        }
                        // Otherwise, we have to overwrite the current value
                        else {
                            int currVal = nexp->getValue(var_map, arr_map);
                            outer_itr->second.erase(inner_itr);
                            outer_itr->second.insert(std::pair<int,int>(index, currVal));
                        }

                    }
                }
                // If a simple variable, assign/overwrite variable with value to var map
                else {
                    std::map<std::string, int>::iterator search_it;
                    search_it = var_map.find(varName);
                    if (search_it == var_map.end()) var_map.insert(std::pair<std::string, int>(varName, assignment_val));
                    else {
                        /* Creating temp to store value to be overwritten.
                        * This is important because we want to be able to access
                        * a variable's value when overwriting its value.
                        * e.g. LET X (X + 1)
                        */
                        int currVal = nexp->getValue(var_map, arr_map);
                        var_map.erase(search_it);
                        var_map.insert(std::pair<std::string, int>(varName, currVal));
                    }
                }
            }
            else if (currCommandType == "GOTO"){
                size_t jline = currCommand->get_size_t_jline();
                if(!lineExists(jline)){
                    printNonExistentLineError(currCommand, currCommandType, jline);
                    map_itr++;
                    break;
                }
                else {
                    map_itr = command_map.find(jline);
                    continue;
                }
            }
            else if (currCommandType == "GOSUB"){
                size_t jline = currCommand->get_size_t_jline();
                if(!lineExists(jline)){
                    printNonExistentLineError(currCommand, currCommandType, jline);
                    map_itr++;
                    break;
                }
                gosub_stack.push(currCommand->getLineNumber());
                map_itr = command_map.find(jline);
                continue;
            }
            else if (currCommandType == "RETURN"){
                if (goSubStackIsEmpty()) {
                    printEmptyStackError(currCommand);
                    map_itr++;
                    break;
                }
                else {
                    size_t jline = gosub_stack.top();
                    gosub_stack.pop();
                    map_itr = command_map.find(jline);
                    //map_itr = findJumpLine(jline);
                    // Iincrementing since RETURN always returns to line after most recent GOSUB
                    map_itr++;
                    continue;
                }
            }
            else if (currCommandType == "END"){
                map_itr++;
                break;
            }
        }
        catch (std::logic_error &e) {
            std::cout << "Error in Line " << currCommand->getLineNumber() << ": ";
            NumericExpression* num_exp = currCommand->getNexp();
            num_exp->printErrorMessage(var_map, arr_map);
            break;
        }
        map_itr++;
    }
    return;
}

// Returns map iterator to line which will be used for GOTO, GOSUB, and IF commands
/* In doing runtime analysis, I realized that the map.find() method was faster, so I
** Replaced all instances of this function with .find().                           */
std::map<size_t, Command*>::iterator Interpreter::findJumpLine(size_t jline){
    std::map<size_t, Command*>::iterator jump_itr;
    int count = 0;
    for (jump_itr = command_map.begin(); jump_itr != command_map.end(); jump_itr++){
        if (jump_itr->first == jline) {
            return jump_itr--;
        }
        count++;
    }
    return command_map.end();
}

// Prints error message for RETURN command with no matching GOSUB
void Interpreter::printEmptyStackError(Command* currCommand){
    std::cout << "Error in Line " << currCommand->getLineNumber() << ": ";
    std::cout << "No matching GOSUB for RETURN." << std::endl;
}

// Returns true if GOSUB stack is empty, false if otherwise.
bool Interpreter::goSubStackIsEmpty(){
    if (gosub_stack.empty()) return true;
    else return false;
}

// Prints error message for non-existent jumpline from IF, GOSUB, and GOTO commands
void Interpreter::printNonExistentLineError(Command* currCommand, std::string currCommandType, size_t jline){
    std::cout << "Error in Line " << currCommand->getLineNumber() << ": ";
    if (currCommandType == "IF") std::cout << "IF jump ";
    else if (currCommandType == "GOSUB") std::cout << "GOSUB ";
    else if (currCommandType == "GOTO") std::cout << "GOTO ";
    std::cout << "to non-existent line " << jline << "." << std::endl; 
    return;
}

// Iterates through map, check if line exists for IF, GOTO, and GOSUB commands.
bool Interpreter::lineExists(size_t  lineNumber){

    std::map<size_t, Command*>::iterator map_itr;
    map_itr = command_map.find(lineNumber);
    if (map_itr == command_map.end()) return false;
    else return true;
}

// Parses input file into backtrace tree.
void Interpreter::parse(std::istream& in) {
    std::string line;
    // Read in each line from string
    while (getline(in, line)) {
        size_t lineNumber;
        std::stringstream stream;
        stream << line;
        stream >> lineNumber;

        std::string command;
        stream >> command;

        // Construct appropriate command object w/ line # based on cmd keyword
        if (command == "PRINT") {
            PrintCommand* p = new PrintCommand(lineNumber);
            NumericExpression* nexp = parse_numeric_expression(stream);
            p->setNexp(nexp);
            command_map.insert(std::pair<size_t, Command*>(lineNumber, p));
            p = nullptr;
        }
        else if (command == "LET") {

            LetVarCommand* p = new LetVarCommand(lineNumber);
            Variable* varName = parse_variable(stream);
            p->setVarName(varName);
            NumericExpression* nexp = parse_numeric_expression(stream);
            p->setNexp(nexp);
            command_map.insert(std::pair<size_t, Command*>(lineNumber, p));
            p = nullptr;    

        }
        else if (command == "GOTO") {
            GotoCommand* p = new GotoCommand(lineNumber);
            Constant* jpointer = parse_constant(stream);
            p->setJLine(jpointer);
            command_map.insert(std::pair<size_t, Command*>(lineNumber, p));
            p = nullptr;
        }
        else if (command == "IF") 
        {
            IfCommand* p = new IfCommand(lineNumber);
            BooleanExpression* bexp = parse_boolean_expression(stream);
            p->setBooleanExpression(bexp);
            
            // Consume "THEN" from stream
            char c;
            stream >> c;
            while (c != 'T') stream >> c;
            std::string s;
            stream >> std::skipws>> s;
            Constant* jpointer = parse_constant(stream);
            p->setJLine(jpointer);
            command_map.insert(std::pair<size_t, Command*>(lineNumber, p));
            p = nullptr;
        }
        else if (command == "GOSUB") 
        {
            GoSubCommand* p = new GoSubCommand(lineNumber);
            Constant* jpointer = parse_constant(stream);
            p->setJLine(jpointer);
            command_map.insert(std::pair<size_t, Command*>(lineNumber, p));
            p = nullptr;
        }
        else if (command == "RETURN") 
        {
            ReturnCommand* p = new ReturnCommand(lineNumber);
            command_map.insert(std::pair<size_t, Command*>(lineNumber, p));
            p = nullptr;
        }
        else if (command == "END") 
        {
            EndCommand* p = new EndCommand(lineNumber);
            command_map.insert(std::pair<size_t, Command*>(lineNumber, p));
            p = nullptr;
        }

    }
}

Constant* Interpreter::parse_constant(std::stringstream& ss){
    //Dynamically allocate new constant
    int value;
    Constant* cpointer = new Constant;
    ss >> value;
    cpointer->setConstantValue(value);
    return cpointer;
}

Variable* Interpreter::parse_variable(std::stringstream& ss){
    //Dynamically allocate new variable name given input
    Variable* varName = new Variable;
    std::string name_string;


    ss >> name_string;
    char c1;

    for (unsigned int i = 0; i < name_string.size(); i++)
    {
        if (!isalpha(name_string[i]))
        {
            for (unsigned int j = name_string.size(); j >= i; j--)
            {
                ss.putback(name_string[j]);
            }

            name_string = name_string.substr(0,i);
            c1 = ss.peek();

            if (c1 == '['){
                ss.get(c1);
                
                //while (ss >> c1) std::cout << c1 << std::endl;
                NumericExpression* nexp = parse_numeric_expression(ss);
                varName->setNexp(nexp);
                if (c1 == ' ' || c1 == 9 || c1 == 0){
                    //removes chars until non-whitespace is reached
                    c1 = ss.peek();
                    while (c1 == ' ' || c1 == 9 || c1 == 0){
                        ss.get(c1);
                        c1 = ss.peek();
                    }
                }
                c1 = ss.peek();

                while (c1 == ']')
                {
                    ss.get(c1);
                    c1 = ss.peek();
                }
                
            }

            varName->setVariableName(name_string);
            return varName;
        }
    }

    c1 = ss.peek();


    if (c1 == ' ' || c1 == 9 || c1 == 0){
        //removes chars until non-whitespace is reached
        c1 = ss.peek();
        while (c1 == ' ' || c1 == 9 || c1 == 0){
            ss.get(c1);
            c1 = ss.peek();
        }
    }

    if (c1 == '['){
        ss.get(c1);
        NumericExpression* nexp = parse_numeric_expression(ss);
        varName->setNexp(nexp);

    }

    varName->setVariableName(name_string);
    return varName;

}

BinaryExpression* Interpreter::parse_binary_expression(std::stringstream& ss){
    //Dynamically allocate new binary expression

    BinaryExpression* bin_exp = new BinaryExpression;

    NumericExpression* left = parse_numeric_expression(ss);

    bin_exp->setNexpLeft(left);

    // Moving through stream to retrieve and set binary operator
    char c = ss.peek();
    while (c != '+' && c != '-' && c != '*' && c != '/') ss >> c;    
    bin_exp->setExpOp(c);
    ss.get(c);

    NumericExpression* right = parse_numeric_expression(ss);
    bin_exp->setNexpRight(right);
    
    // Consume close paren
    ss >> c;

    return bin_exp;
}


NumericExpression* Interpreter::parse_numeric_expression(std::stringstream& ss){
    //Dynamically allocate new nexp given input
    char c1;
    ss.get(c1);

    if (c1 == ' ' || c1 == 9 || c1 == 0){
        // removes chars until non-whitespace is reached
        c1 = ss.peek();
        while (c1 == ' ' || c1 == 9){
            ss.get(c1);
            c1 = ss.peek();
        }
    }
    else
    {
        ss.putback(c1);
        c1 = ss.peek();
    }

    // calls Numeric Expression
    if (c1 == '('){
        ss.get(c1);
        BinaryExpression* binExp = parse_binary_expression(ss);
        c1 = ss.peek();
        return binExp; 
    }
    else if (isalpha(c1)){
        Variable* varName = parse_variable(ss);
        c1 = ss.peek();
        while (c1 == ']')
        {
            ss.get(c1);
        }
        return varName;
    }
    else if (isdigit(c1) || c1 == '-'){
        Constant* cpointer = parse_constant(ss);
        c1 = ss.peek();
        while (c1 == ']')
        {
            ss.get(c1);
        }
        return cpointer;
    }

    // skips closing paren's to find next nexp
    while (c1 == ')')
    {
        c1 = ss.peek();
        if (c1 != ')') break;
        ss.get(c1);
    }

    return nullptr;
}

BooleanExpression* Interpreter::parse_boolean_expression(std::stringstream& ss){
    BooleanExpression* bexp = new BooleanExpression;
    
    NumericExpression* left = parse_numeric_expression(ss);
    bexp->setLeftNexp(left);
    left = nullptr;

    char c;
    ss >> c;

    while (c != '<' && c != '>' && c != '=')
    {
        ss >> c;
    }
    //std::cout << c << std::endl;
    // Consume end parens
    /*while (c == ')')
    {
        ss >> c;
    }*/


    bexp->setBoolOperator(c);

    NumericExpression* right = parse_numeric_expression(ss);
    bexp->setRightNexp(right);
    right = nullptr;

    // Consume end parens
    while (c == ')')
    {
        ss >> c;
    }

    return bexp;
}


// Output pretty-printed parsed lines of program
void Interpreter::write(std::ostream& out) {
    Command* to_write;
    std::map<size_t, Command*>::iterator map_itr;
    for (map_itr = command_map.begin(); map_itr != command_map.end(); map_itr++){
        to_write = map_itr->second;
        std::cout << to_write->format() << std::endl;
    }
    return;
}
