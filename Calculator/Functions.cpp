#include "Functions.h"
#include "Variable.h"
#include "Errors.h"
#include "Gconst.h"
#include "Token.h"
#include <vector>
#include <string>
#include <iostream>



using std::vector;
using std::string;

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