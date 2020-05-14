#pragma once

#include <string>

//------------------------------------------------------------------------------
// Defines 3 Tokens of different types: a character token "Token(char ch)" with no value,
// a number token "Token(char ch, double val)" with an associated numeric value,
// and a string token "Token(char ch, string n)" with an associated name string.

class Token
{
public:
    char kind;
    double value;
    string name;
    Token() :kind{ 0 } {}
    Token(char ch) :kind{ ch } {}
    Token(char ch, double val) :kind{ ch }, value{ val }{}
    Token(char ch, string n) :kind{ ch }, name{ n }{}
};