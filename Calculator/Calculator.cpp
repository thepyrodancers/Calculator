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
#include <vector>
#include "Variable.h"
#include "Errors.h"
#include "Gconst.h"
#include "Token.h"

using std::cin;
using std::cout;
using std::vector;
using std::cerr;
using std::exception;

//------------------------------------------------------------------------------
// Creates a "Token buffer" where a token can be temporarily placed or held for later evaluation.
// The next token read from the input stream can be retrieved using Token get(),
// a token can be placed back in the buffer after analyis with putback(Token t), 
// and a token can be overwritten or ignored with ignore(char c)
// When "bool full" is true a token is currently in the buffer and can't be overwritten
// When "bool full" is set to false the current token can be overwritten with the next token

class Token_stream {
public:
    Token_stream(); 
    Token get(); 
    void putback(Token t); 
    void ignore(char c);
private:
    bool full;
    Token buffer;

};

//------------------------------------------------------------------------------
// This constructor initializes Token_stream's buffer to be empty and overwritable

Token_stream::Token_stream()
    :full(false), buffer(0)
{
}


//------------------------------------------------------------------------------
// Evaluates the buffer and assembles and returns a Token(char ch), Token(char ch , double val),
// or Token(char ch, string n) depending on the input being read

Token Token_stream::get()
{
    if (full) {
        full = false; // Buffer can be overwritten
        return buffer; // Returns token currently in buffer
    }
    char ch;
    cin >> ch;
    switch (ch) {
        case quit:
        case print:
        case '!':
        case '{':
        case '}':
        case '(':
        case ')':
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '=':
        case ',':
        {
            return Token{ ch };
        }
        case '.': case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        {
            cin.putback(ch);
            double val;
            cin >> val;
            return Token{ number,val };
        }
        default: 
        {
            if (isalpha(ch)) {
                string s;
                s += ch;
                while (cin.get(ch) && (isalpha(ch) || isdigit(ch))) {
                    s += ch;
                }
                cin.putback(ch);
                if (s == declkey) {
                    return Token{ let };
                }
                if (s == sqrtkey) {
                    return Token{ root };
                }
                if (s == powkey) {
                    return Token{ power };
                }
                return Token{ name, s };
            }
            error("Bad token");
        }
    }
}

//------------------------------------------------------------------------------
// The current token is placed back into the buffer after evaluation so that it may
// further evaluated by proceeding functions

void Token_stream::putback(Token t)
{
    if (full) error("putback() into a full buffer");
    buffer = t; 
    full = true; //Buffer cannot be overwritten
}

//------------------------------------------------------------------------------
// The character passed to this function is evaluated against the current token in
// the buffer and if they are a match 'bool full' is set to false which allows the
// buffer to be overwritten. This essentially ignores the passed character in an evaluation

void Token_stream::ignore(char c)
{
    if (full && c == buffer.kind) {
        full = false;
        return;
    }
    full = false;
    char ch = 0;
    while (cin >> ch) {
        if (ch == c) {
            return;
        }
    }
}



//------------------------------------------------------------------------------
// Gives user a summary of the calculator program and states the rules/instructions for use
// Defines mathematical constants such as "pi"
// Begins the evaluation of user input by calling calculate()
// Handles all exceptions and errors outside of those defined within the calculator program

int main()
try {
    cout << "Welcome to our simple calculator. \nPlease enter expressions using floating-point numbers.\n";
    cout << "You may use '+' for addition, '-' for subtraction (or negative numbers), '*' for multiplication,\n";
    cout << "'/' for division, '%' for modulo, or follow a number or variable with '!' to get its factorial.\n";
    cout << "You may use '(', ')','{', & '}' to denote modifications to the normal order of mathematical operations.\n";
    cout << "You may define a variable by typing 'let (your variable here) = (your expression here)'.\n";
    cout << "Variables must start with an alpha character and contain no spaces.\n";
    cout << "End your expressions with ';' to get a result. Enter 'q' to quit at any time.\n\n";
    
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