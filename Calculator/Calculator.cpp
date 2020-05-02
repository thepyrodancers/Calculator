/*
 Aaron's Calculator Code

This program implements a basic expression calculator.
Input from cin; output to cout.
The grammar for input is :
Statement:
    Expression
    Print
    Quit
Print :
    ;
Quit  :
    q
Expression :
    Term
    Expression + Term
    Expression – Term
Term :
    Primary
    Term * Primary
    Term / Primary
    Term % Primary
Primary :
    Number
    (Expression)
    – Primary
    + Primary
Number :
    floating - point - literal

Input comes from cin through the Token_stream called ts.
*/

#include "Facilities.h"



//------------------------------------------------------------------------------
// Defines Token(ch), Token (ch, val), and Token (ch, string)

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

class Token_stream {
public:
    Token_stream(); 
    Token get(); 
    void putback(Token t); 
    void ignore(char c);
private:
    bool full; // Tracks if buffer can be overwritten or not
    Token buffer; // Receives token from .putback(Token t)

};

//------------------------------------------------------------------------------

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
// Evaluates the buffer and assembles and returns Token(ch), Token(ch,val), Token  or Token depending 
// on character input

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

void Token_stream::putback(Token t)
{
    if (full) error("putback() into a full buffer");
    buffer = t; 
    full = true; //Buffer cannot be overwritten
}

//------------------------------------------------------------------------------

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

class Variable {
public:
    string name;
    double value;
    Variable(string n, double val)
        : name(n), value(val) {}
};

//------------------------------------------------------------------------------

vector<Variable> var_table;

//------------------------------------------------------------------------------

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

bool is_declared(string var)
{
    for (int i = 0; i < var_table.size(); ++i)
        if (var_table[i].name == var) {
            return true;
        }
    return false;
}

//------------------------------------------------------------------------------

double define_name(string var, double val)
{
    if (is_declared(var)) {
        error(var, " declared twice");
    }
    var_table.push_back(Variable{ var, val });
    return val;
}

//------------------------------------------------------------------------------



Token_stream ts;

//------------------------------------------------------------------------------

double expression();

//------------------------------------------------------------------------------

double squareroot()
{
    double e = expression();
    double f = sqrt(e);
    if (e < 0) {
        error("Square Root of Negative");
    }
    return f;
}

//------------------------------------------------------------------------------

double powerfunc()
{
    
    ts.ignore('(');
    Token t = ts.get();
    double x = t.value;
    ts.ignore(',');
    Token t2 = ts.get();
    int i = narrow_cast<int>(t2.value);
    ts.ignore(')');
    return pow(x, i);
}

//------------------------------------------------------------------------------



// Gets next token from the token stream and evaluates expressions grouped within
// '{}' or '()' as well as single number expressions and returns the value 
// for the grouped expressions or single numbers

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
// Gets next token from token stream and evaluates a term. Calculates multiplication or division
// of a term by a factorial or primary and returns the result. All tokens other than '*' and '/'
// are put back into the token stream

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
            case ',':
            {
                return left, t, narrow_cast<int> (term());
            }
            default:
                ts.putback(t); 
                return left; 
        }
    }
}

//------------------------------------------------------------------------------

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

void clean_up_mess()
{
    ts.ignore(print);
}

//------------------------------------------------------------------------------

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