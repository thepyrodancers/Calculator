/*
Simple Calculator

This program implements a basic expression calculator.
Input comes from cin through the Token_stream called "ts"; utput to cout.
The grammar for input is :
Calculation:
    Statement
    Print ';'
    Quit 'q'
Statement:
    Declaration (of variable)
    Expression
Expression :
    Term
    Expression + Term
    Expression – Term
Term :
    Primary
    Term * Primary
    Term / Primary
    Term % Primary
Factorial:
    Primary()!
Primary :
    Number
    (Expression)
    – Primary
    + Primary
    Name (variable)
    sqrt(Expression)
    pow(x,i)
Number :
    floating - point - literal
*/

#include <iostream>
#include "Errors.h"
#include "Functions.h"
#include "Gconst.h"

using std::cin;
using std::cout;
using std::cerr;
using std::exception;

//------------------------------------------------------------------------------
// Gives user a summary of the calculator program and states the rules/instructions for use
// Defines mathematical constants such as "pi"
// Begins the evaluation of user input by calling calculate()
// Handles all exceptions and errors outside of those defined within the calculator program

int main()
try {
    cout << "       D(umb) & A(ss) Enterprises\n               presents: \n    A COMPLEX SIMPLE CALCULATOR!\n\n";
    cout << "You may enter 'help' for instructions or 'q' to quit at anytime.\n\n" << prompt;
    define_name("pi", 3.1415926535);
    define_name("e", 2.7182818284);
    define_name("k", 1000);

    calculate();
    return 0;
}
catch (exception& e) {
    cerr << e.what() << '\n';
    return 1;
} 
catch (...) {
    cerr << "exception \n";
    return 2;
}