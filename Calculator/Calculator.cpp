/*
Tip: THE CODE BELOW IS EXAMPLE CODE. FOR LEARNING PURPOSES ONLY
*/

/*
Tip: Tips are just that, tips. Everything else is comments I
would call acceptable with my limited experience
*/

/*
Tip: I think you typically start with all or your #includes, no
comment necessary here unless one is unique. or you might group
them. we'll get to that later
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
by the user and calculates and displays the results

*/

/*
Tip: Let's start by defining our user defined types (classes)
and their member functions. This would typically be done
elsewhere and each class would have its own file
*/

/*
Class: Token

Description: This class will be used to hold user input as a
two part object. One for its kind, the other for its value. 
It comes equipped with two initializers, one for characters
that have no value, the other for ones that do
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

/*
Class: Token_stream

Description: This class will be used as a buffer to hold a token.
It comes equipped with two public functions, one to get a token
from user input or the buffer. the other to put a token in the
buffer. two private variables will be needed by for member functions
to carry out their task. One is a Token that is the actual buffer,
the other is a bool that keeps track of whether or not the buffer
is full
*/

class Token_stream {
public:
    Token_stream();
    Token get();
    void putback(Token t);
private:
    bool full;
    Token buffer;
};

/*
Tip: Don't quote me on this, but this class needs a constructor to initialize
its private variable. I think constuctors are usually a good idea anyway
*/

/*
Token_stream constructor: initializes buffer to empty
*/

Token_stream::Token_stream()
    :full(false), buffer(0)
{
}

/*
Token_stream member function get(): gets a token from the buffer or a 
character from the user input stream and assigns it to the appropriate 
token
*/

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

/*
Token_stream member function putback(): puts a token in the buffer
*/

void Token_stream::putback(Token t)
{
    if (full) error("putback() into a full buffer");
    buffer = t;
    full = true;
}

/*
Tip: Let's go ahead a do some declaring. we'll go ahead and declare all of 
our functions know, that way they can be written in any order and
organized below. This is usually handled in a more sophisticated way, but 
this will work for now
*/

/*
Basic functions
*/

double primary();
double factorial();
double term();
double expression();

/*
utility functions //tip: this should grow
*/

double calculate_factorial(int val);

/*
Global variables
*/

Token_stream ts;

/*
Function: primary()

Description: This is the last step in the grammar chain,
it checks for primaries, which include {, (, and numbers.
it will then either return the number to factorial() or evaluate 
another expression and then return a number to term()
*/

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

/*
Function: factorial()

Description: This funcions is used to check if a '!' has been 
found after a number has been read in or an expression has 
been evaluated. if so, return the factorial of the number or
evaluated expression to term()
*/

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

/*
Function: term()

Description: this function handles multiplication and division.
if a * or / are found, evaluate for factorial and primary and
multiply by the previous factorial or primary. return the result 
to expression()
*/

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

/*
Function: expression()

Description: This function handles + and -. if one is found, it appropriately
combines the previous term() with the next term() and returns the result
*/

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

/*
Function: calculate_factorial()

Description: This function takes a number, calculate its
factorial, and returns its number
*/

double calculate_factorial(int val)
{
    for (int i = val - 1; i > 0; i--)
        val *= i;
    return val;
}

/*
Function: main()

Description: main function of program. displays instructions on load.
Continuously runs program while input is valid. checks for quit characters
and print characters.
*/

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

