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
// A table for holding user declared variables

vector<Variable> var_table;

//------------------------------------------------------------------------------
// Initializes the Token_stream "ts"

Token_stream ts;

double get_value(string s)
{
    for (int i = 0; i < var_table.size(); ++i) {
        if (var_table[i].name == s) {
            return var_table[i].value;
        }
    }
    error("get: undefined variable ", s);
}

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

bool is_declared(string var)
{
    for (int i = 0; i < var_table.size(); ++i)
        if (var_table[i].name == var) {
            return true;
        }
    return false;
}

double define_name(string var, double val)
{
    if (is_declared(var)) {
        error(var, " declared twice");
    }
    var_table.push_back(Variable{ var, val });
    return val;
}

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

double factorial() 
{
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

void clean_up_mess()
{
    ts.ignore(print);
}

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