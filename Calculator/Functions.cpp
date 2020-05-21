#include <iostream>
#include <vector>
#include <Windows.h>
#include "conio.h"
#include "Variable.h"
#include "Errors.h"
#include "Gconst.h"
#include "Token.h"
#include "Functions.h"
#include "Token_stream.h"

using std::cin;
using std::cout;
using std::vector;
using std::cerr;
using std::exception;

//------------------------------------------------------------------------------
// Declares a table "var_table" used for holding user declared variables

vector<Variable> var_table;

//------------------------------------------------------------------------------
// Calculation Functions Section
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Searches through vector "var_table" for argument "string s"; if a match
// is found, returns the value property of that vector element
// If a match for argument "string s" is not found "print" (;) is returned to the input stream
// and an error is thrown

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
// Searches through vector "var_table" for argument "string s"; if a match
// is found, the value property of that vector member is set to argument "double d"
// If a match for argument "string s" is not found "print" (;) is returned to the input stream
// and an error is thrown

double reset_value(Token_stream& myts)
{
    Token t = myts.get();
    if (t.kind != name) {
        error("name expected in declaration");
    }
    string var_name = t.name;
    Token t2 = myts.get();
    if (t2.kind != '=') {
        error("= missing in declaration of ", var_name);
    }
    double d = expression(myts);
    for (int i = 0; i < var_table.size(); ++i) {
        if (var_table[i].name == var_name) {
            var_table[i].value = d;
            return d;
        }
    }
    error("set: undefined variable ", var_name);
}

//------------------------------------------------------------------------------
// Searches through vector "var_table" for argument "string var"; if a match is
// found, bool is_declared(string var) returns true and if a match is not found
// returns false

bool is_declared(string var)
{
    for (int i = 0; i < var_table.size(); ++i)
        if (var_table[i].name == var) {
            return true;
        }
    return false;
}

//------------------------------------------------------------------------------
// Calls function "is_declared" passing the argument "string var" to check if 
// argument "string var" has already been declared (returns true). If it has been 
// declared an error is thrown. If argument "string var" has NOT been previously declared
// (returns false), it is then added to vector "var_table" along with the value property argument
//  "double val" and value property "double val" is returned

double define_name(string var, double val)
{
    if (is_declared(var)) {
        error(var, " previously declared; use 'reset' to change the value.");
    }
    var_table.push_back(Variable{ var, val });
    return val;
}

//------------------------------------------------------------------------------
// "double c" is set to the result of function "expression()", Token t is set to
// the next token in Token_stream 'ts', which is returned by  function "ts.get()"
// If the kind property of the token is not a ')' an error is thrown
// "double c" is returned

double parenthesis(Token_stream& myts)
{
    double c = expression(myts);
    Token t = myts.get();
    if (t.kind != ')') {
        error("')' expected");
    }
    return c;
}

//------------------------------------------------------------------------------
// "double c" is set to the result of function "expression()", Token t is set to
// the next token in Token_stream 'ts', which is returned by  function "ts.get()"
// If the "kind" property of the token is not a '}' an error is thrown
// "double c" is returned

double braces(Token_stream& myts)
{
    double c = expression(myts);
    Token t = myts.get();
    if (t.kind != '}') {
        error("'}' expected");
    }
    return c;
}

//------------------------------------------------------------------------------
// Gets tokens made from user input from Token_stream. Returns errors if incorrect syntax is used.
// Evaluates the expression within the parenthesis of the "sqrt()" user input.
// Returns the squareroot of the expression.

double squareroot(Token_stream& myts)
{
    Token t = myts.get();
    if (t.kind != '(') {
        error("'(' expected");
    }
    double e = expression(myts);
    if (e < 0) {
        error("Square Root of Negative");
    }
    Token t2 = myts.get();
    if (t2.kind != ')') {
        error("')' expected");
    }
    return sqrt(e);
}

//------------------------------------------------------------------------------
// Gets tokens made from user input from Token_stream. Returns errors if incorrect syntax is used.
// Evaluates the expression within the parenthesis of the user input pow() and returns 
// that expression the the power specified by the user.

double powerfunc(Token_stream& myts)
{
    Token t = myts.get();
    if (t.kind != '(') {
        error("'(' expected");
    }
    double x = expression(myts);
    Token t2 = ts.get();
    if (t2.kind != ',') {
        error("',' expected");
    }
    int i = narrow_cast<int>(expression(myts));
    Token t3 = ts.get();
    if (t3.kind != ')') {
        error("')' expected");
    }
    return pow(x, i);
}

//------------------------------------------------------------------------------
// Multiplies each number by the next lowest number starting with the user input until 1 is reached.
// Example: 5! = 5*4*3*2*1
// Returns the result.

double calculate_fac(double fact)
{
    
    double x = fact;
    double f = x;
    for (int i = 0; i < fact - 1; ++i) {
        f *= --x;
    }
    return f;
}

//------------------------------------------------------------------------------
// Sets variable "left" in term() to the result of factorial() multiplied by "left" 
// as it is passed by reference.
// Gets next token from Token_stream, which is passed by reference back to term()

void multiply(double& left, Token& t, Token_stream& myts)
{
    left *= factorial(myts);
    t = myts.get();
}

//------------------------------------------------------------------------------
// Sets double 'd' to the result of factorial(). Variable "left" from term() is 
// set to "left" divided by double 'd' as it is passed by reference.
// Gets next token from Token_stream, which is passed by reference back to term().

void divide(double& left, Token& t, Token_stream& myts)
{
    double d = factorial(myts);
    if (d == 0) error("divide by zero");
    left /= d;
    t = myts.get();
}

//------------------------------------------------------------------------------
//

void modulo(double& left, Token& t, Token_stream& myts)
{
    int i1 = narrow_cast<int>(left);
    int i2 = narrow_cast<int>(factorial(myts));
    if (i2 == 0) error("%: divide by zero");
    left = i1 % i2;
    t = myts.get();
}

//------------------------------------------------------------------------------
//

void add(double& left, Token& t, Token_stream& myts)
{
    left += term(myts);
    t = myts.get();
}

//------------------------------------------------------------------------------
//

void subtract(double& left, Token& t, Token_stream& myts)
{
    left -= term(myts);
    t = myts.get();
}

//------------------------------------------------------------------------------
//

void helpdisplay()
{
    cout << "\nYou may enter expressions using integers or floating - point numbers.\n";
    cout << "You may use '{', '}','(', & ')' to denote modifications to the normal order of mathematical operations.\n";
    cout << "End your expressions with ';' to get a result.\n\n";
    cout << "Operators:\n";
    cout << "   Addition: '+', Subtraction: '-', Multiplication: '*', Division: '/', Modulo: '%',\n";
    cout << "   Squareroot: 'sqrt(your expression here)', Power: 'pow(your expression here, exponent integer here)',\n";
    cout << "   Factorial: your expression followed by '!' \n";
    cout << "Examples: 1+{2-(3*4/5)}; sqrt(9); pow(3,2); 3!; -5+-5; 10%3;\n\n";
    cout << "Variables:\n";
    cout << "   You may define a variable by typing 'let (your variable here) = (your expression here)' Example: let x=5;.\n";
    cout << "   Variables must start with an alpha character and contain no spaces.\n";
    cout << "   To redeclare a variable use 'reset (your variable here) = (your expression here)' Example: reset x=6;.\n\n";
    cout << "Included mathematical constants:\n";
    cout << "   'pi' (3.1415926535), 'e' (2.7182818284), 'k' (1000)\n\n";
}

//------------------------------------------------------------------------------
//

void clean_up_mess(Token_stream& myts)
{
    myts.ignore(print);
}

//------------------------------------------------------------------------------
// Parsing Functions Section
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//

double primary(Token_stream& myts)
{
    Token t = myts.get();
    switch (t.kind) {
    case '{':
    {
        return braces(myts);
    }
    case '(':
    {
        return parenthesis(myts);
    }

    case '-':
    {
        return -primary(myts);
    }
    case '+':
    {
        return primary(myts);
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
        return squareroot(myts);
    }
    case power:
    {
        return powerfunc(myts);
    }
    default:
        error("primary expected");
    }
}

//------------------------------------------------------------------------------
//

double factorial(Token_stream& myts)
{
    double left = primary(myts);
    Token t = myts.get();
    while (true) {

        switch (t.kind) {
        case '!':
        {
            return calculate_fac(left);
        }
        default:
            myts.putback(t);
            return left;
        }
    }
}

//------------------------------------------------------------------------------
//

double term(Token_stream& myts)
{
    double left = factorial(myts);
    Token t = myts.get();
    while (true) {
        switch (t.kind)
        {
        case '*':
        {
            multiply(left, t, myts);
            break;
        }
        case '/':
        {
            divide(left, t, myts);
            break;
        }
        case '%':
        {
            modulo(left, t, myts);
            break;
        }

        default:
            myts.putback(t);
            return left;
        }
    }
}

//------------------------------------------------------------------------------
//

double expression(Token_stream& myts)
{
    double left = term(myts);
    Token t = myts.get();
    while (true) {
        switch (t.kind) {
        case '+':
        {
            add(left, t, myts);
            break;
        }
        case '-':
        {
            subtract(left, t, myts);
            break;
        }
        default:
            myts.putback(t);
            return left;
        }
    }
}

//------------------------------------------------------------------------------
//

double declaration(Token_stream& myts)
{
    Token t = myts.get();
    if (t.kind != name) {
        error("name expected in declaration");
    }
    string var_name = t.name;
    Token t2 = myts.get();
    if (t2.kind != '=') {
        error("= missing in declaration of ", var_name);
    }
    double d = expression(myts);
    define_name(var_name, d);
    return d;
}

//------------------------------------------------------------------------------
//

double statement(Token_stream& myts)
{
    Token t = myts.get();
    switch (t.kind) {
    case let:
    {
        return declaration(myts);
    }
    case reset:
    {
        return reset_value(myts);
    }
    default:
        myts.putback(t);
        return expression(myts);
    }
}

void calculate()
{
    while (cin) {
        cout << prompt;
        while (cin) {
            try {
                Token t = myts.get();
                while (t.kind == print) {
                    t = myts.get();
                }
                if (t.kind == help) {
                    helpdisplay();
                    break;
                }
                if (t.kind == quit) {
                    return;
                }
                myts.putback(t);
                cout << result << statement(myts) << '\n';
                if (cin.get() == '\n') {
                    break;
                }
                else
                    cin.unget();
            }
            catch (exception& e) {
                cerr << e.what() << '\n';
                cin.putback(print);
                clean_up_mess();
            }
        }
    }
}