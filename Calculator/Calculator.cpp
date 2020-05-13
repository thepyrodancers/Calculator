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

#include "Facilities.h"



//------------------------------------------------------------------------------
// Defines 3 Tokens of different types: a character token "Token(char ch)" with no value,
// a number token "Token(char ch, double val)" with an associated numeric value,
// and a string token "Token(char ch, string n)" with an associated name string.

class Token {
public:
    char kind;
    double value;
    string name;
    Token(char ch)
        :kind(ch), value(0) { }
    Token(char ch, double val)     
        :kind(ch), value(val) { }
    Token(char ch, string n)
        :kind(ch), name(n) { } 
};

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

//-----------------------------------------------------------------------------

const char number = 'n';
const char quit = 'q';
const char print = ';';
const string prompt = ">";
const string result = "=";
const char name = 'a';
const char let = 'L';
const string declkey = "let";
const char root = 's';
const string sqrtkey = "sqrt";
const char power = 'p';
const string powkey = "pow";


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
// Creates a variable consisting of a string (which must start with an alpha character)
// and a numeric value (which may be the result of an expression)

class Variable {
public:
    string name;
    double value;
    Variable(string n, double val)
        : name(n), value(val) {}
};

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
                for (int i = 0; i < left-1; ++i) {
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
    keep_window_open();
    return 0;
}
catch (exception& e) {
    cerr << e.what() << endl;
    keep_window_open("~~");
    return 1;
} 
catch (...) {
    cerr << "exception \n";
    keep_window_open("~~");
    return 2;
}