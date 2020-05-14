#pragma once

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
// Returns the square root of the user's inputted expression

double squareroot();

//------------------------------------------------------------------------------
// Returns input 'x' to the power of input 'i'
// The '(', ')', and ',' in the input "pow(x,i)" are ignored and not treated 
// as an expression. Only the user's inputted numeric values are used in the function

double powerfunc();

//------------------------------------------------------------------------------
// Gets next token from the token stream and evaluates expressions grouped within
// '{}' or '()' as well as single number expressions and returns the value 
// for the grouped expressions or single numbers
// Also handles negative numbers by returning -primary() when a '-' is used before a number
// Returns the numeric value for a user defined variable string when it is used in an expression
// Calls squareroot() and powerfunc() when the user has inputted "sqrt()" or "pow()"

double primary();

//------------------------------------------------------------------------------
// Gets next token from token stream and calculates a factorial denoted by '!' and
// returns the value for the factorial. All tokens other than '!' are put back into the
// token stream

double factorial();

//------------------------------------------------------------------------------
// Gets next token from token stream and evaluates a term. Calculates multiplication, division,
// or modulo of a term by a factorial or primary and returns the result. All tokens other than '*', '/',
// and '%' are put back into the token stream

double term();

//------------------------------------------------------------------------------
// Gets next token from the token stream and evaluates expressions. Calculates 
// addition and subtraction of terms within the expression. All tokens other than
// '+' and '-' are returned to the token stream

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
// This is called when an error is caught within an expression so that any following expressions
// are evaluated without the erroneous expression's 'print' character included in the evaluation
// Passes the 'print' character ';' to ignore() ensuring it is not included in proceeding 
// expression evaluations 

void clean_up_mess();

//------------------------------------------------------------------------------
// Gives the user an input prompt and begins creation, evaluation, and storing of tokens
// in the buffer 
// Outputs result of each individual expression ended with the "print" character ';'
// Results of expressions are evaluated beginning with the call of statement()
// Checks for "quit" character 'q' and ends program when it is used
// Checks for errors in individual expressions and outputs error messages while allowing
// the evaluation of all other expressions by calling clean_up_mess() 

void calculate();
