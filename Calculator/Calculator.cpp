

// Aaron's Calculator Code 

#include "Facilities.h"

//------------------------------------------------------------------------------

class Token {
public:
    char kind;        // what kind of token
    double value;     // for numbers: a value 
    Token(char ch)    // make a Token from a char
        :kind(ch), value(0) { }
    Token(char ch, double val)     // make a Token from a char and a double
        :kind(ch), value(val) { }
};

//------------------------------------------------------------------------------

class Token_stream {
public:
    Token_stream(); // make a Token_stream that reads from cin
    Token get(); // get a Token
    void putback(Token t); // put a Token back
private:
    bool full{ false }; // is there a Token in the buffer?
    Token buffer; // here is where we keep a Token put back using putback()

};


//------------------------------------------------------------------------------

// The constructor just sets full to indicate that the buffer is empty:
Token_stream::Token_stream()
    :full(false), buffer(0)    // no Token in buffer
{
}

//------------------------------------------------------------------------------

void Token_stream::putback(Token t)
{
    if (full) error("putback() into a full buffer");
    buffer = t; // copy t to buffer
    full = true; // buffer is now full
}

//------------------------------------------------------------------------------

Token Token_stream::get()
{
    if (full) { // do we already have a Token ready?
        full = false; // remove Token from buffer
        return buffer;
    }
    char ch;
    cin >> ch; // note that >> skips whitespace (space, newline, tab, etc.)
    switch (ch) {
        case '=': // for “print”
        case 'x': // for “quit”
        case '!': // for "factorial"
        case '{': case '}': case '(': case ')': case '+': case '-': case '*': case '/':
        {
            return Token{ch}; // let each character represent itself
        }
        case '.':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        { 
            cin.putback(ch); // put digit back into the input stream
            double val;
            cin >> val; // read a floating-point number
            return Token{'n',val}; // let ‘n’ represent “a number”}
        }
        default:
        {
            error("Bad token"); 
        } 
    }
}

//------------------------------------------------------------------------------

Token_stream ts;       // provides get() and putback()

//------------------------------------------------------------------------------

double expression();  // read and evaluate a Expression

//------------------------------------------------------------------------------

//double factorial() // read and evaluate a factorial
//{
//    Token t = ts.get();
//    switch (t.kind) {
//    case '!':
//    {
//
//    }
//    }
//}

//------------------------------------------------------------------------------

double primary()     // read and evaluate a Primary
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
    case '(':    // handle '(' expression ')'
    {
        double d = expression();
        t = ts.get();
        if (t.kind != ')') error("')' expected");
        return d;
    }
    case 'n':         // we use 'n' to represent a number
    {
        return t.value;  // return the number's value
    }
    default:
        error("primary expected");
    }
}

//------------------------------------------------------------------------------

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

double term()
{
    double left = factorial();
    Token t = ts.get(); // get the next Token from the Token stream
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
            ts.putback(t); // put t back into the Token stream
            return left;
        }
    }
}


//------------------------------------------------------------------------------

double expression()
{
    double left = term(); // read and evaluate a Term
    Token t = ts.get(); // get the next Token from the Token stream
    while (true) {
        switch (t.kind) {
        case '+':
            left += term(); // evaluate Term and add
            t = ts.get();
            break;
        case '-':
            left -= term(); // evaluate Term and subtract
            t = ts.get();
            break;
        default:
            ts.putback(t); // put t back into the token stream
            return left; // finally: no more + or –; return the answer
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