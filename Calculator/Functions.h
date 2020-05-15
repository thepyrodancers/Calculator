#pragma once
#include "Token.h"

//------------------------------------------------------------------------------
// Searches through the variable table to find the inputted variable and returns its
// corresponding numeric value. The variable must be previously declared by the user

double get_value(string s);

//------------------------------------------------------------------------------
// Associates a numeric value with a user declared variable string, 

void set_value(string s, double d);

//------------------------------------------------------------------------------
// Checks to see if a string  has already been declared as a variable
// A specific string my only be declared as a variable once

bool is_declared(string var);

//------------------------------------------------------------------------------
// Adds a new user defined string variable to the table of variables if
// that string hasn't already been declared by the user previously

double define_name(string var, double val);

//------------------------------------------------------------------------------
// Evaluates expressions grouped within '{}'and returns the value 


double braces();

//------------------------------------------------------------------------------
// Evaluates expressions grouped within '()'and returns the value

double parenthesis();

//------------------------------------------------------------------------------
// Returns the square root of the user's inputted expression

double squareroot();

//------------------------------------------------------------------------------
// Returns input 'x' to the power of input 'i'
// The '(', ')', and ',' in the input "pow(x,i)" are ignored and not treated 
// as an expression. Only the user's inputted numeric values are used in the function

double powerfunc();

//------------------------------------------------------------------------------
// Calculates the factorial of a primary and returns the result

double calculatefac(double fac);

//------------------------------------------------------------------------------
// Calculates multiplication of a term by a factorial or primary and returns the result.

void multiply(double& left, Token& t);

//------------------------------------------------------------------------------
// Calculates division of a term by a factorial or primary and returns the result.

void divide(double& left, Token& t);

//------------------------------------------------------------------------------
// Calculates modulo of a term by a factorial or primary and returns the result.

void modulo(double& left, Token& t);

//------------------------------------------------------------------------------
// Calculates addition of terms within expression.

void add(double& left, Token& t);

//------------------------------------------------------------------------------
// Calculates subtraction of terms within expression.

void subtract(double& left, Token& t);

//------------------------------------------------------------------------------
// Displays calclulator instructions when user enters "help"

void helpdisplay();

//------------------------------------------------------------------------------
// This is called when an error is caught within an expression so that any following expressions
// are evaluated without the erroneous expression's 'print' character included in the evaluation
// Passes the 'print' character ';' to ignore() ensuring it is not included in proceeding 
// expression evaluations 

void clean_up_mess();

//------------------------------------------------------------------------------
// Grammar Functions Section
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Gets next token from the token stream. Calls various functions depending on the token type.
// Handles expressions within braces or parenthesis by calling braces() or parenthesis()
// Handles negative numbers by returning -primary() when a '-' is placed before a number or expresion
// Returns the numeric value of a number token
// Calls get_value() to return the numeric value for a user defined variable string
// Calls squareroot() and powerfunc() when the user has inputted "sqrt()" or "pow()"
// Returns error if incorrect syntax is used

double primary();

//------------------------------------------------------------------------------
// Gets next token from token stream and calls calculatefac() when a factioral denoted by '!' is used
// All tokens other than '!' are put back into the token stream

double factorial();

//------------------------------------------------------------------------------
// Gets next token from token stream and evaluates a term. Calls multiply(), divide(),
// or modulo() when multiplication, diviion, or modulo denoted by '*', '/', or '%' are used
// All tokens other than '*', '/', and '%' are put back into the token stream

double term();

//------------------------------------------------------------------------------
// Gets next token from the token stream and evaluates expressions. Calls add() and 
// subract() when addition or subtraction denoted by '+' and '-' are used
// All tokens other than '+' and '-' are returned to the token stream

double expression();

//------------------------------------------------------------------------------
// Creates user declared string variable with its associated numerical value
// and passes it to define_name() to be added to the variable table
// Checks that user has properly declared the variable with the format "x = expression"
// Returns the numerical value of the user defined string variable

double declaration();

//------------------------------------------------------------------------------
// Gets the next token from the token stream and if it is a "let" token (i.e. user input "let x = 3")
// returns declaration(), which creates a user declared variable from the user's inputted string and numeric value
// All other tokens are returned to the token stream for further evaluation

double statement();

//------------------------------------------------------------------------------
// Gives the user an input prompt and begins creation, evaluation, and storing of tokens
// in the buffer 
// Outputs result of each individual expression ended with the "print" character ';'
// Results of expressions are evaluated beginning with the call of statement()
// Checks for "quit" character 'q' and ends program when it is used
// Checks for "help" string and calls helpdisplay() when it is used
// Checks for errors in individual expressions and outputs error messages while allowing
// the evaluation of all other expressions by calling clean_up_mess() 

void calculate();
