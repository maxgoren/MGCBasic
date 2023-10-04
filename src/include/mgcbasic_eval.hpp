/*
MIT License

Copyright (c) 2023 Max Goren, maxgoren@gmail.com http://www.maxgcoding.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#ifndef mgcbasic_eval_hpp
#define mgcbasic_eval_hpp
#include <iostream>
#include <cmath>
#include "Stack.hpp"
#include "mgcbasic_globals.hpp"
using std::pair;
using std::make_pair;
using std::string;

/// extract number from string starting at spos
pair<int, double> parseNumber(string str, int spos) {
    int e = spos;
    while (isdigit(str[e]) || str[e] == '.') ++e;
    return make_pair(e-1, atof(str.substr(spos, e).c_str()));
}

/// evaluate algebraic expression supplied as string
/// using dijkstras 2 stack algorithm. supports negative numbers.
double eval(string expr) {
    string str;
    setInterpreterState(EXECUTING);
    if (expr[1] != '(')
        str = "(" + expr + ")";
    else 
        str = expr;
    char op;
    Stack<double> vals;
    Stack<char> ops;
    for (int i = 0; i < str.size(); i++) {
        if      (str[i] == '(') {
        } else if (str[i] == '+') ops.push(str[i]);
        else if (str[i] == '*') ops.push(str[i]);
        else if (str[i] == '/') ops.push(str[i]);
        else if (str[i] == '%') ops.push(str[i]);
        else if (str[i] == '-' && str[i+1] == ' ') ops.push(str[i]);
        else if (str[i] == '=' && str[i+1] == '=') ops.push(str[i]);
        else if (str[i] == '!' && str[i+1] == '=') ops.push(str[i]);
        else if (str[i] == '<' || str[i] == '>')   ops.push(str[i]);
        else if (str[i] == ')') { 
            if (!ops.empty()) op = ops.pop();
            double v = vals.pop();
            if      (op == '+') v = vals.pop() + v;
            else if (op == '*') v = vals.pop() * v;
            else if (op == '-') v = vals.pop() - v;
            else if (op == '/') v = vals.pop() / v;
            else if (op == '!') v = vals.pop() != v;
            else if (op == '=') v = vals.pop() == v;
            else if (op == '<') v = vals.pop() < v;
            else if (op == '>') v = vals.pop() > v;
            vals.push(v);
        } else if (isdigit(str[i]) || (str[i] == '-' && isdigit(str[i+1]))) {
            auto t = parseNumber(str, i);
            vals.push(t.second);
            i = t.first;
        } 
    }
    return vals.pop();
}

#endif