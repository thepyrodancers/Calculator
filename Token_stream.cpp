#include "Token.h"
#include "Gconst.h"
#include <iostream>
#include "Token_stream.h"
#include <string>
#include "Errors.h"

using std::cin;
using std::string;


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