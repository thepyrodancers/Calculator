/*
Tip: THE CODE BELOW IS EXAMPLE CODE. FOR LEARNING PURPOSES ONLY
*/

/*
Tip: I think you would typically start with all you #includes, no
comment necessary
*/

#include "Facilities.h"

/*
Tip: after that, you could give this shitty code a general 
description so people will know what it is without having to 
look further
*/

/*

Calculator program:

This program reads simple mathematical expressions entered
by the user and calculates and displays the results..

*/

/*
Tip: Let's start by defining our user defined types (classes)
and their member functions. This would typically be done
elsewhere and each class would have its own file.
*/

class Token {
public:
    char kind;
    double value;
    Token(char ch)
        :kind(ch), value(0) { }
    Token(char ch, double val)
        :kind(ch), value(val) { }
};

class Token_stream {
public:
    Token_stream();
    Token get();
    void putback(Token t);
private:
    bool full;
    Token buffer;
};

Token_stream::Token_stream()
    :full(false), buffer(0)
{
}

void Token_stream::putback(Token t)
{
    if (full) error("putback() into a full buffer");
    buffer = t;
    full = true;
}

Token Token_stream::get()
{
    if (full) {
        full = false;
        return buffer;
    }

    char ch;
    cin >> ch;

    switch (ch) {
    case ';':
    case 'q':
    case '!': case '{': case '}': case '(': case ')': case '+': case '-': case '*': case '/':
        return Token(ch);
    case '.':
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    {
        cin.putback(ch);
        double val;
        cin >> val;
        return Token('n', val);
    }
    default:
        error("Bad token");
    }
}

double primary();
double factorial();
double term();
double expression();

double calculate_factorial(int val);

Token_stream ts;

double primary()
{
    Token t = ts.get();
    switch (t.kind) 
    {
    case '{':
        {
            double d = expression();
            t = ts.get();
            if (t.kind != '}') error("'}' expected");
            return d;
        }
    case '(':
        {
            double d = expression();
            t = ts.get();
            if (t.kind != ')') error("')' expected");
            return d;
        }
    case 'n':
        {
            double d = t.value;
            return d;
        }
    case 'q':
        {
            exit(0);
            return 0;
        }
    default:
        {
            error("primary expected");
        }
    }
}

double factorial()
{
    double left = primary();
    Token t = ts.get();

    while (true)
    {
        switch (t.kind)
        {
        case '!':
        {
            left = calculate_factorial(left);
            return left;
        }
        default:
            ts.putback(t);
            return left;
        }
        return left;
    }
}

double term()
{
    double left = factorial();
    Token t = ts.get();

    while (true) {
        switch (t.kind) {
        case '*':
            left *= factorial();
            t = ts.get();
            break;
        case '/':
        {
            double d = factorial();
            if (d == 0) error("divide by zero");
            left /= d;
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
            left += term();
            t = ts.get();
            break;
        case '-':
            left -= term();
            t = ts.get();
            break;
        default:
            ts.putback(t);
            return left;
        }
    }
}

double calculate_factorial(int val)
{
    for (int i = val - 1; i > 0; i--)
        val *= i;
    return val;
}

int main()
try {
    cout << "Welcome to our simple calculator. Please enter expressions using \n";
    cout << "floating-point numbers. enter ';' to calculate and display, 'q' to \n";
    cout << "quit. The following operators are supported : \n";
    cout << "   + - * / ( ) \n";
    double val = 0;
    while (cin)
    {
        Token t = ts.get();

        if (t.kind == 'q') break;
        if (t.kind == ';')
            cout << "=" << val << '\n';
        else
            ts.putback(t);
        val = expression();
    }
}
catch (exception& e) {
    cerr << e.what() << endl;
    return 1;
}
catch (...) {
    cerr << "exception \n";
    return 2;
}

