#include "Functions.h"
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>




//------------------------------------------------------------------------------
// A table for holding user declared variables

vector<Variable> var_table;

//------------------------------------------------------------------------------
// Searches through the variable table to find the inputted variable and returns its
// corresponding numeric value. The variable must be previously declared by the user

double get_value(string s)
{
    for (int i = 0; i < var_table.size(); ++i) {
        if (var_table[i].name == s) {
            return var_table[i].value;
        }
    }
    error("get: undefined variable ", s);
}

//------------------------------------------------------------------------------
// Associates a numeric value with a user declared variable string, 

void set_value(string s, double d)
{
    for (int i = 0; i < var_table.size(); ++i) {
        if (var_table[i].name == s) {
            var_table[i].value = d;
            return;
        }
        error("set: undefined variable ", s);
    }
}

//------------------------------------------------------------------------------
// Checks to see if a string  has already been declared as a variable
// A specific string my only be declared as a variable once

bool is_declared(string var)
{
    for (int i = 0; i < var_table.size(); ++i)
        if (var_table[i].name == var) {
            return true;
        }
    return false;
}

//------------------------------------------------------------------------------
// Adds a new user defined string variable to the table of variables if
// that string hasn't already been declared by the user previously

double define_name(string var, double val)
{
    if (is_declared(var)) {
        error(var, " declared twice");
    }
    var_table.push_back(Variable{ var, val });
    return val;
}

//------------------------------------------------------------------------------
// Initializes the Token_stream "ts"

Token_stream ts;

//------------------------------------------------------------------------------
// Initializes the expression() function for SOME REASON THAT I MUST FIND AND ADD HERE!!!!

double expression();

//------------------------------------------------------------------------------
// Returns the square root of the user's inputted expression

double squareroot()
{
    Token t = ts.get();
    if (t.kind != '(') {
        error("'(' expected");
    }
    ts.putback(t);
    ts.ignore('(');
    double e = expression();
    if (e < 0) {
        error("Square Root of Negative");
    }
    Token t2 = ts.get();
    if (t2.kind != ')') {
        error("')' expected");
    }
    ts.putback(t2);
    ts.ignore(')');
    return sqrt(e);
}

//------------------------------------------------------------------------------
// Returns input 'x' to the power of input 'i'
// The '(', ')', and ',' in the input "pow(x,i)" are ignored and not treated 
// as an expression. Only the user's inputted numeric values are used in the function

double powerfunc()
{
    Token t = ts.get();
    if (t.kind != '(') {
        error("'(' expected");
    }
    ts.putback(t);
    ts.ignore('(');
    double x = expression();
    Token t2 = ts.get();
    if (t2.kind != ',') {
        error("',' expected");
    }
    ts.putback(t2);
    ts.ignore(',');
    int i = narrow_cast<int>(expression());
    Token t3 = ts.get();
    if (t3.kind != ')') {
        error("')' expected");
    }
    ts.putback(t3);
    ts.ignore(')');
    return pow(x, i);
}

//------------------------------------------------------------------------------



// Gets next token from the token stream and evaluates expressions grouped within
// '{}' or '()' as well as single number expressions and returns the value 
// for the grouped expressions or single numbers
// Also handles negative numbers by returning -primary() when a '-' is used before a number
// Returns the numeric value for a user defined variable string when it is used in an expression
// Calls squareroot() and powerfunc() when the user has inputted "sqrt()" or "pow()"

double primary()
{

    Token t = ts.get();
    switch (t.kind) {
    case '{':
    {
        double c = expression();
        t = ts.get();
        if (t.kind != '}') {
            error("'}' expected");
        }
        return c;
    }
    case '(':
    {
        double d = expression();
        t = ts.get();
        if (t.kind != ')') {
            error("')' expected");
        }
        return d;
    }

    case '-':
    {
        return -primary();
    }
    case '+':
    {
        return primary();
    }
    case number:
    {
        return t.value;
    }
    case name:
    {
        return get_value(t.name);
    }
    case root:
    {
        return squareroot();
    }
    case power:
    {
        return powerfunc();
    }
    default:
        error("primary expected");
    }
}

//------------------------------------------------------------------------------
// Gets next token from token stream and calculates a factorial denoted by '!' and
// returns the value for the factorial. All tokens other than '!' are put back into the
// token stream

double factorial() {
    double left = primary();
    Token t = ts.get();
    while (true) {

        switch (t.kind) {
        case '!':
        {
            double x = left;
            double f = x;
            for (int i = 0; i < left - 1; ++i) {
                f *= --x;
            }
            return f;
        }
        default:
            ts.putback(t);
            return left;
        }
    }
}

//------------------------------------------------------------------------------
// Gets next token from token stream and evaluates a term. Calculates multiplication, division,
// or modulo of a term by a factorial or primary and returns the result. All tokens other than '*', '/',
// and '%' are put back into the token stream

double term()
{
    double left = factorial();
    Token t = ts.get();
    while (true) {
        switch (t.kind)
        {
        case '*':
        {
            left *= factorial();
            t = ts.get();
            break;
        }
        case '/':
        {
            double d = factorial();
            if (d == 0) error("divide by zero");
            left /= d;
            t = ts.get();
            break;
        }
        case '%':
        {
            int i1 = narrow_cast<int>(left);
            int i2 = narrow_cast<int>(factorial());
            if (i2 == 0) error("%: divide by zero");
            left = i1 % i2;
            t = ts.get();
            break;
        }

        default:
            ts.putback(t);
            return left;
        }
    }
}


//------------------------------------------------------------------------------
// Gets next token from the token stream and evaluates expressions. Calculates 
// addition and subtraction of terms within the expression. All tokens other than
// '+' and '-' are returned to the token stream

double expression()
{
    double left = term();
    Token t = ts.get();
    while (true) {
        switch (t.kind) {
        case '+':
        {
            left += term();
            t = ts.get();
            break;
        }
        case '-':
        {
            left -= term();
            t = ts.get();
            break;
        }
        default:
            ts.putback(t);
            return left;
        }
    }
}

//------------------------------------------------------------------------------
// Creates user declared string variable with its associated numerical value
// and passes it to define_name() to be added to the variable table
// Checks that user has properly declared the variable with the format "x = expression"
// Returns the numerical value of the user defined string variable

double declaration()
{
    Token t = ts.get();
    if (t.kind != name) {
        error("name expected in declaration");
    }
    string var_name = t.name;
    Token t2 = ts.get();
    if (t2.kind != '=') {
        error("= missing in declaration of ", var_name);
    }
    double d = expression();
    define_name(var_name, d);
    return d;
}

//------------------------------------------------------------------------------
// Gets the next token from the token stream and if it is a "let" token (i.e. user input "let x = 3")
// returns declaration(), which creates a user declared variable from the user's inputted string and numeric value
// All other tokens are returned to the token stream for further evaluation

double statement()
{
    Token t = ts.get();
    switch (t.kind) {
    case let:
    {
        return declaration();
    }
    default:
        ts.putback(t);
        return expression();
    }
}

//------------------------------------------------------------------------------
// This is called when an error is caught within an expression so that any following expressions
// are evaluated without the erroneous expression's 'print' character included in the evaluation
// Passes the 'print' character ';' to ignore() ensuring it is not included in proceeding 
// expression evaluations 

void clean_up_mess()
{
    ts.ignore(print);
}

//------------------------------------------------------------------------------
// Gives the user an input prompt and begins creation, evaluation, and storing of tokens
// in the buffer 
// Outputs result of each individual expression ended with the "print" character ';'
// Results of expressions are evaluated beginning with the call of statement()
// Checks for "quit" character 'q' and ends program when it is used
// Checks for errors in individual expressions and outputs error messages while allowing
// the evaluation of all other expressions by calling clean_up_mess() 

void calculate()
{
    while (cin)
        try {
        cout << prompt;
        Token t = ts.get();
        while (t.kind == print) {
            t = ts.get();
        }
        if (t.kind == quit) {
            return;
        }
        ts.putback(t);
        cout << result << statement() << '\n';
    }
    catch (exception& e) {
        cerr << e.what() << '\n';
        clean_up_mess();
    }
}