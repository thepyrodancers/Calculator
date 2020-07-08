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
// is found, returns the value property of that vector element.
// If a match for argument "string s" is not found "print" (;) is returned to the input stream
// and an error is thrown.

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
// is found, the value property of that vector member is set to argument "double d".
// If a match for argument "string s" is not found "print" (;) is returned to the input stream
// and an error is thrown.

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
// returns false.

bool is_declared(string var, vector<Variable>& var_table)
{
    for (int i = 0; i < var_table.size(); ++i)
        if (var_table[i].name == var) {
            return true;
        }
    return false;
}

//------------------------------------------------------------------------------
// Calls function "is_declared" passing the user inputted argument "string var" to check if 
// it has already been declared (returns true). If it has already been 
// declared an error is thrown. If argument "string var" has NOT been previously declared
// (returns false), it is then added to vector "var_table" along with the value property argument
//  "double val" and value property "val" is returned.

double define_name(string var, double val, vector<Variable>& var_table)
{
    if (is_declared(var, var_table)) {
        error(var, " previously declared; use 'reset' to change the value.");
    }
    var_table.push_back(Variable{ var, val });
    return val;
}

//------------------------------------------------------------------------------
// Evaluates an expression following a '(' token. If the token following the expression
// is not a ')' an error is thrown. The result of the expression within the parenthesis or
// "expr" is returned.

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
// Evaluates an expression following a '{' token. If the token following the expression
// is not a '}' an error is thrown. The result of the expression within the braces or
// "expr" is returned.

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

//------------------------------------------------------------------------------
// Returns a primary as a negative value when denoted by the user with a preceeding '-'.
// An error is thrown if a '-' is used with no following primary.

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

//------------------------------------------------------------------------------
// Returns a primary as a positive value when unneccessarily denoted by the user with a preceeding '+'.
// An error is thrown if a '+' is used with no following primary.

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
// Returns the squareroot of the expression "expr".

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
// Evaluates the user inputted expression within the parenthesis of the user input "pow()" and returns 
// that expression "expr" to the power of the "exponent" specified by the user. "exponent" is narrow cast
// as an integer so floating-point decimal numbers will not be accepted as an exponent.

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

void multiply(double& left, Token& tkn, Token_stream& tkn_strm, vector<Variable>& var_table)
{
    left *= factorial(tkn_strm, var_table);
    tkn = tkn_strm.get();
}

//------------------------------------------------------------------------------
// Sets double 'expr' to the result of factorial(). Variable "left" from term() is 
// set to "left" divided by double 'd' as it is passed by reference.
// Gets next token from Token_stream, which is passed by reference back to term().

void divide(double& left, Token& tkn, Token_stream& tkn_strm, vector<Variable>& var_table)
{
    double expr;

    expr = factorial(tkn_strm, var_table);
    if (expr == 0) error("divide by zero");
    left /= expr;
    tkn = tkn_strm.get();
}

//------------------------------------------------------------------------------
// Sets double 'expr1' to "left" from term(). Sets double 'expr2' to the result of factorial().
// "left" in term() is then set to the result of the modulo operation of "expr1" and "expr2"
// as it is passed by reference. Floating-point decimals are not used in modulo operation so
// both expressions are narrow cast as integers. An error is thrown if division by 0 occurs.
// "tkn" is set to the next token from the Token_stream and is passed by reference back to term().

void modulo(double& left, Token& tkn, Token_stream& tkn_strm, vector<Variable>& var_table)
{
    int expr1;
    int expr2;

    expr1 = narrow_cast<int>(left);
    expr2= narrow_cast<int>(factorial(tkn_strm, var_table));
    if (expr2 == 0) error("%: divide by zero");
    left = expr1 % expr2;
    tkn = tkn_strm.get();
}

//------------------------------------------------------------------------------
// Variable "left" from expression() is added to the result of term() and then passed by reference
// back to expression(). "tkn" is set to the next token from the Token_stream and is passed by 
// reference back to expression() as well.

void add(double& left, Token& tkn, Token_stream& tkn_strm, vector<Variable>& var_table)
{
    left += term(tkn_strm, var_table);
    tkn = tkn_strm.get();

}

//------------------------------------------------------------------------------
// The result of term() is subtracted from the variable "left" from expression() and then passed by reference
// back to expression(). "tkn" is set to the next token from the Token_stream and is passed by 
// reference back to expression() as well.

void subtract(double& left, Token& tkn, Token_stream& tkn_strm, vector<Variable>& var_table)
{
    left -= term(tkn_strm, var_table);
    tkn = tkn_strm.get();
}

//------------------------------------------------------------------------------
// The instructions for user input and operations are outputted to the console.

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
// When an expression in a list of expressions is found to have an error
// the ';' or "print" character following the erroroneous expression is passed to tkn_strm.ignore
// and is removed from the evaluation of the proceeding expressions.

void clean_up_mess(Token_stream& tkn_strm)
{
    tkn_strm.ignore(print);
}

//------------------------------------------------------------------------------
// Parsing Functions Section
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Gets next token from the token stream. Calls various functions depending on the token type.
// Handles expressions within braces or parenthesis by calling braces() or parenthesis().
// Handles negative numbers by returning -primary() when a '-' is placed before a number or expression.
// Returns the numeric value of a number token.
// Calls get_value() to return the numeric value for a user defined variable string.
// Calls squareroot() and powerfunc() when the user has inputted "sqrt()" or "pow()".
// Returns error if incorrect syntax is used.

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
// Gets next token from token stream and calls calculatefac() when a factioral denoted by '!' is used.
// All tokens other than '!' are put back into the token stream.

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
// Gets next token from token stream and evaluates a term. Calls multiply(), divide(),
// or modulo() when multiplication, diviion, or modulo denoted by '*', '/', or '%' are used.
// All tokens other than '*', '/', and '%' are put back into the token stream.

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
// Gets next token from the token stream and evaluates expressions. Calls add() and 
// subract() when addition or subtraction denoted by '+' and '-' are used.
// All tokens other than '+' and '-' are returned to the token stream.

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
// Creates user declared string variable with its associated numerical value
// and passes it to define_name() to be added to the variable table.
// Checks that user has properly declared the variable with the format "x = expression"
// Returns the numerical value of the user defined string variable.

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
// Gets the next token from the token stream and if it is a "let" token (i.e. user input "let x = 3")
// returns declaration(), which creates a user declared variable from the user's inputted string and numeric value.
// If the token is a "reset" token (i.e. user input "reset x = 4") returns reset_value(), which updates
// the numeric value of the user's previously declared string Variable.
// All other tokens are returned to the token stream for further evaluation.

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

//------------------------------------------------------------------------------
//  Begins creation, evaluation, and storing of tokens in the buffer.
// Results of expressions are evaluated beginning with the call of statement().
// Outputs result of each individual expression ended with the "print" character ';'.
// Checks for "quit" character 'q' and ends program when it is used.
// Checks for "help" string and calls helpdisplay() when it is used.
// Checks for errors in individual expressions and outputs error messages while allowing.
// the evaluation of all other expressions by calling clean_up_mess(). 

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