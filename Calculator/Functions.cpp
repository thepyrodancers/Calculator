#include <iostream>
#include <vector>
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
// Calculation Functions Section
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Searches through vector "var_table" for argument "string s"; if a match
// is found, returns the value property of that vector element
// If a match for argument "string s" is not found "print" (;) is returned to the input stream
// and an error is thrown

double get_value(string s, vector<Variable>& var_table)
{
    for (int i = 0; i < var_table.size(); ++i) {
        if (var_table[i].name == s) {
            return var_table[i].value;
        }
    }
    cin.putback(print);
    error("get: undefined variable ", s);
}

//------------------------------------------------------------------------------
// Searches through vector "var_table" for argument "string s"; if a match
// is found, the value property of that vector member is set to argument "double d"
// If a match for argument "string s" is not found "print" (;) is returned to the input stream
// and an error is thrown

double reset_value(Token_stream& tkn_strm, vector<Variable>& var_table)
{
    Token tkn;
    Token tkn2;
    double expr;
    string var_name;

    tkn = tkn_strm.get();
    if (tkn.kind != name) {
        cin.putback(print);
        error("name expected in declaration");
    }
    var_name = tkn.name;
    tkn2 = tkn_strm.get();
    if (tkn2.kind != '=') {
        cin.putback(print);
        error("= missing in declaration of ", var_name);
    }
    expr = expression(tkn_strm, var_table);
    for (int i = 0; i < var_table.size(); ++i) {
        if (var_table[i].name == var_name) {
            var_table[i].value = expr;
            return expr;
        }
    }
    error("reset: undefined variable ", var_name);
}

//------------------------------------------------------------------------------
// Searches through vector "var_table" for argument "string var"; if a match is
// found, bool is_declared(string var) returns true and if a match is not found
// returns false

bool is_declared(string var, vector<Variable>& var_table)
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

double define_name(string var, double val, vector<Variable>& var_table)
{
    if (is_declared(var, var_table)) {
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

double parenthesis(Token_stream& tkn_strm, vector<Variable>& var_table)
{
    Token tkn;
    double expr;

    expr = expression(tkn_strm, var_table);
    tkn = tkn_strm.get();
    if (tkn.kind != ')') {
        cin.putback(print);
        error("')' expected");
    }
    return expr;
}

//------------------------------------------------------------------------------
// "double c" is set to the result of function "expression()", Token t is set to
// the next token in Token_stream 'ts', which is returned by  function "ts.get()"
// If the "kind" property of the token is not a '}' an error is thrown
// "double c" is returned

double braces(Token_stream& tkn_strm, vector<Variable>& var_table)
{
    Token tkn;
    double expr;

    expr = expression(tkn_strm, var_table);
    tkn = tkn_strm.get();
    if (tkn.kind != '}') {
        cin.putback(print);
        error("'}' expected");
    }
    return expr;
}

double neg_exp(Token_stream& tkn_strm, vector<Variable>& var_table)
{
    Token tkn;

    tkn = tkn_strm.get();
    if (tkn.kind == '-') {
        error("Error: Primary expected (-)");
    }
    else {
        tkn_strm.putback(tkn);
        return -primary(tkn_strm, var_table);
    }
}

double pos_exp(Token_stream& tkn_strm, vector<Variable>& var_table)
{
    Token tkn;

    tkn = tkn_strm.get();
    if (tkn.kind == '+') {

        error("Error: Primary expected (+)");

    }
    else {
        tkn_strm.putback(tkn);
        return primary(tkn_strm, var_table);
    }
}

//------------------------------------------------------------------------------
// Gets tokens made from user input from Token_stream. Returns errors if incorrect syntax is used.
// Evaluates the expression within the parenthesis of the "sqrt()" user input.
// Returns the squareroot of the expression.

double squareroot(Token_stream& tkn_strm, vector<Variable>& var_table)
{
    Token tkn;
    Token tkn2;
    double expr;

    tkn = tkn_strm.get();
    if (tkn.kind != '(') {
        error("'(' expected");
    }
    expr = expression(tkn_strm, var_table);
    if (expr < 0) {
        cin.putback(print);
        error("Square Root of Negative");
    }
    tkn2 = tkn_strm.get();
    if (tkn2.kind != ')') {
        tkn_strm.putback(tkn2);
        error("')' expected");
    }
    return sqrt(expr);
}

//------------------------------------------------------------------------------
// Gets tokens made from user input from Token_stream. Returns errors if incorrect syntax is used.
// Evaluates the expression within the parenthesis of the user input pow() and returns 
// that expression the the power specified by the user.

double powerfunc(Token_stream& tkn_strm, vector<Variable>& var_table)
{
    Token tkn;
    Token tkn2;
    Token tkn3;
    double expr;
    int exponent;

    tkn = tkn_strm.get();
    if (tkn.kind != '(') {
        error("'(' expected");
    }
    expr = expression(tkn_strm, var_table);
    tkn2 = tkn_strm.get();
    if (tkn2.kind != ',') {
        tkn_strm.putback(tkn2);
        cin.putback(print);
        error("',' expected");
    }
    exponent = narrow_cast<int>(expression(tkn_strm, var_table));
    tkn3 = tkn_strm.get();
    if (tkn3.kind != ')') {
        tkn_strm.putback(tkn3);
        error("')' expected");
    }
    return pow(expr, exponent);
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

void multiply(double& left, Token& t, Token_stream& tkn_strm, vector<Variable>& var_table)
{
    left *= factorial(tkn_strm, var_table);
    t = tkn_strm.get();
}

//------------------------------------------------------------------------------
// Sets double 'd' to the result of factorial(). Variable "left" from term() is 
// set to "left" divided by double 'd' as it is passed by reference.
// Gets next token from Token_stream, which is passed by reference back to term().

void divide(double& left, Token& t, Token_stream& tkn_strm, vector<Variable>& var_table)
{
    double expr;

    expr = factorial(tkn_strm, var_table);
    if (expr == 0) error("divide by zero");
    left /= expr;
    t = tkn_strm.get();
}

//------------------------------------------------------------------------------
//

void modulo(double& left, Token& t, Token_stream& tkn_strm, vector<Variable>& var_table)
{
    int expr1;
    int expr2;

    expr1 = narrow_cast<int>(left);
    expr2= narrow_cast<int>(factorial(tkn_strm, var_table));
    if (expr2 == 0) error("%: divide by zero");
    left = expr1 % expr2;
    t = tkn_strm.get();
}

//------------------------------------------------------------------------------
//

void add(double& left, Token& t, Token_stream& tkn_strm, vector<Variable>& var_table)
{
    left += term(tkn_strm, var_table);
    t = tkn_strm.get();

}

//------------------------------------------------------------------------------
//

void subtract(double& left, Token& t, Token_stream& tkn_strm, vector<Variable>& var_table)
{
    left -= term(tkn_strm, var_table);
    t = tkn_strm.get();
}

//------------------------------------------------------------------------------
//

void helpdisplay()
{
    cout << "\nYou may enter expressions using integers or floating - point numbers.\n";
    cout << "You may use '{', '}','(', & ')' to denote modifications to the normal order of mathematical operations.\n";
    cout << "You may enter multiple expressions seperated by ';'.\n\n";
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

void clean_up_mess(Token_stream& tkn_strm)
{
    tkn_strm.ignore(print);
}

//------------------------------------------------------------------------------
// Parsing Functions Section
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//

double primary(Token_stream& tkn_strm, vector<Variable>& var_table)
{
    Token tkn;

    tkn = tkn_strm.get();
    switch (tkn.kind) {
    case '{':
    {
        return braces(tkn_strm, var_table);
    }
    case '(':
    {
        return parenthesis(tkn_strm, var_table);
    }

    case '-':
    {
        return neg_exp(tkn_strm, var_table);
    }
    case '+':
    {
        return pos_exp(tkn_strm, var_table);
    }
    case number:
    {
        return tkn.value;
    }
    case name:
    {
        return get_value(tkn.name, var_table);
    }
    case root:
    {
        return squareroot(tkn_strm, var_table);
    }
    case power:
    {
        return powerfunc(tkn_strm, var_table);
    }
    default:
        cin.unget();
        error("Error: Primary expected");
        cin.putback(print);
    }
}

//------------------------------------------------------------------------------
//

double factorial(Token_stream& tkn_strm, vector<Variable>& var_table)
{
    Token tkn;
    double left;

    left = primary(tkn_strm, var_table);
    tkn = tkn_strm.get();
    while (true) {

        switch (tkn.kind) {
        case '!':
        {
            return calculate_fac(left);
        }
        default:
            tkn_strm.putback(tkn);
            return left;
        }
    }
}

//------------------------------------------------------------------------------
//

double term(Token_stream& tkn_strm, vector<Variable>& var_table)
{
    Token tkn;
    double left;

    left = factorial(tkn_strm, var_table);
    tkn = tkn_strm.get();
    while (true) {
        switch (tkn.kind)
        {
        case '*':
        {
            multiply(left, tkn, tkn_strm, var_table);
            break;
        }
        case '/':
        {
            divide(left, tkn, tkn_strm, var_table);
            break;
        }
        case '%':
        {
            modulo(left, tkn, tkn_strm, var_table);
            break;
        }

        default:
            tkn_strm.putback(tkn);
            return left;
        }
    }
}

//------------------------------------------------------------------------------
//

double expression(Token_stream& tkn_strm, vector<Variable>& var_table)
{
    Token tkn; 
    double left;

    left = term(tkn_strm, var_table);
    tkn= tkn_strm.get();
    while (true) {
        switch (tkn.kind) {
        case '+':
        {
            add(left, tkn, tkn_strm, var_table);
            break;
        }
        case '-':
        {
            subtract(left, tkn, tkn_strm, var_table);
            break;
        }
        default:
            tkn_strm.putback(tkn);
            return left;
        }
    }
}

//------------------------------------------------------------------------------
//

double declaration(Token_stream& tkn_strm, vector<Variable>& var_table)
{
    Token tkn;
    Token tkn2;
    string var_name;
    double expr;

   tkn = tkn_strm.get();
    if (tkn.kind != name) {
        cin.putback(print);
        error("name expected in declaration");
    }
    var_name = tkn.name;
    tkn2 = tkn_strm.get();
    if (tkn2.kind != '=') {
        cin.putback(print);
        error("= missing in declaration of ", var_name);
    }
    expr = expression(tkn_strm, var_table);
    define_name(var_name, expr, var_table);
    return expr;
}

//------------------------------------------------------------------------------
//

double statement(Token_stream& tkn_strm, vector<Variable>& var_table)
{
    Token tkn;

    tkn = tkn_strm.get();
    switch (tkn.kind) {
    case let:
    {
        return declaration(tkn_strm, var_table);
    }
    case reset:
    {
        return reset_value(tkn_strm, var_table);
    }
    default:
        tkn_strm.putback(tkn);
        return expression(tkn_strm, var_table);
    }
}

void calculate(Token_stream& tkn_strm, vector<Variable> var_table)
{
    Token tkn;

    cout << prompt;
    while (cin){
        try {
            tkn = tkn_strm.get();
            while (tkn.kind == print) {
                tkn = tkn_strm.get();
            }
            if (tkn.kind == help) {
                helpdisplay();
            }
            if (tkn.kind == quit) {
                return;
            }
            if (tkn.kind != help) {
                tkn_strm.putback(tkn);
                cout << result << statement(tkn_strm, var_table) << '\n';
            }
            if (cin.get() == '\n') {
                cout << prompt;
            }
            else
                cin.unget();
        }
        catch (exception& e) {
            cerr << e.what() << '\n';
                clean_up_mess(tkn_strm);
            if (cin.get() == '\n') {
                cout << prompt;
            }
            else {
                cin.unget();
            }
        }
    }
}