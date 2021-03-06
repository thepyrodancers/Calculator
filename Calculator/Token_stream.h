#pragma once
#include "Token.h"

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