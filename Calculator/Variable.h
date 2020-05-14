#pragma once
#include <string>

using std::string;

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