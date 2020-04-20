

// Aaron's Calculator Code 

#include "Facilities.h"

//------------------------------------------------------------------------------
// Defines Token(ch) and Token (ch,val)

class Token {
public:
    char kind;        
    double value;     
    Token(char ch)    
        :kind(ch), value(0) { }
    Token(char ch, double val)     
        :kind(ch), value(val) { }
};

//------------------------------------------------------------------------------

class Token_stream {
public:
    Token_stream(); 
    Token get(); 
    void putback(Token t); 
private:
    bool full; // Tracks if buffer can be overwritten or not
    Token buffer; // Receives token from .putback(Token t)

};

//------------------------------------------------------------------------------

Token_stream::Token_stream()   
    :full(false), buffer(0)    
{
}

//------------------------------------------------------------------------------

void Token_stream::putback(Token t)
{
    if (full) error("putback() into a full buffer");
    buffer = t; 
    full = true; //Buffer cannot be overwritten
}

//------------------------------------------------------------------------------
// Evaluates the buffer and assembles and returns Token(ch) or Token(ch,val) depending 
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
        case '=': 
        case 'x': 
        case '!': 
        case '{': case '}': case '(': case ')': case '+': case '-': case '*': case '/':
        {
            return Token{ch}; 
        }
        case '.':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        { 
            cin.putback(ch); 
            double val;
            cin >> val; 
            return Token{'n',val}; 
        }
        default:
        {
            error("Bad token"); 
        } 
    }
}

//------------------------------------------------------------------------------

Token_stream ts;       

//------------------------------------------------------------------------------

double expression();  

//------------------------------------------------------------------------------
// Gets next token from the token stream and evaluates expression grouped within
// '{}' or '()' as well as single number expressions and returns the value 
// for the grouped expression or single number

double primary()     
{
    
    Token t = ts.get();
    switch (t.kind) {
    case '{':
    {
        double c = expression();
        t = ts.get();
        if (t.kind != '}') error("'}' expected");
        return c;
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
        return t.value;  
    }
    default:
        error("primary expected");
    }
}

//------------------------------------------------------------------------------
// Gets next token from token stream and calculates a factorial denoted by '!' and
// returns the value for the factorial. All tokens other than '!' are putback into the
// token stream

double factorial() {
    double left = primary();
    Token t = ts.get();
    while (true) {

        switch (t.kind) {
        case '!':
        {
            int x = left;
            int k = (x - 1);
            double f = x;
            for (int i = 0; i < k; ++i) {
                f *= (--x);
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
        switch (t.kind) {
        case '*':
        {
            left *= factorial();
            t = ts.get();
            break;
        }
        case '/':
        { double d = factorial();
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

//------------------------------------------------------------------------------


int main()
try {

    cout << "Welcome to our simple calculator. \nPlease enter expressions using floating-point numbers.\n";
    cout << "You may use '+' for addition, '-' for subtraction, '*' for multiplication, and '/' for division.\n";
    cout << "You may use '(', ')','{', & '}' to denote modifications to the normal order of mathematical operations.\n";
    cout << "End your expressions with '=' to get the result.\n\n";
    cout << ">";
    double val = 0;
    while (cin) {
        Token t = ts.get();
        if (t.kind == 'x') break; // ‘x’ for “quit”
        if (t.kind == '=') // ‘;’ for “print now”
            cout << " " << val << '\n';
        else
            ts.putback(t);
        val = expression();
    }
    keep_window_open("~0");
}
catch (exception& e) {
    cerr << e.what() << endl;
    keep_window_open("~1");
    return 1;
}
catch (...) {
    cerr << "exception \n";
    keep_window_open("~2");
    return 2;
}