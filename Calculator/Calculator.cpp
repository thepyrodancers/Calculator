/*
 Aaron's Calculator Code

This program implements a basic expression calculator.
Input from cin; output to cout.
The grammar for input is :
Calculation:
    Statement
    Print ';'
    Quit 'q'
Statement:
    Declaration
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

Input comes from cin through the Token_stream called ts.
*/

#include <iostream>
#include "Errors.h"
#include "Functions.h"

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
    cout << "Welcome to our simple calculator. \nYou may enter expression using integers or floating-point numbers.\n";
    cout << "Addition: '+', Subtraction: '-', Multiplication: '*', Division: '/', Modulo: '%',\n";
    cout << "Squareroot: 'sqrt(your expression here)', Power: 'pow(your expression here, exponent integer here)',\n";
    cout << "Factorial: your expression followed by '!' \n";
    cout << "You may use '(', ')','{', & '}' to denote modifications to the normal order of mathematical operations.\n";
    cout << "You may define a variable by typing 'let (your variable here) = (your expression here)'.\n";
    cout << "Variables must start with an alpha character and contain no spaces.\n";
    cout << "End your expressions with ';' to get a result. Enter 'q' to quit at any time.\n";
    cout << "Included mathematical constants: 'pi' (3.1415926535), 'e' (2.7182818284), 'k' (1000)\n\n";
    
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