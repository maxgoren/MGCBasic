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
#ifndef eval_expr_hpp
#define eval_expr_hpp
#include <iostream>
#include <stack>
#include <vector>
#include <iomanip>
#include <fstream>
#include "mgcbasic_globals.hpp"
#include "Stack.hpp"
using namespace std;

int m_add(int a, int b) {
    //cout<<a<<" + "<<b<<" = "<<(a +b )<<endl;
    return (a + b);
}

int m_mul(int a, int b) {
    //cout<<a<<" * "<<b<<" = "<<(a * b)<<endl;
    return (a * b);
}

int m_sub(int a, int b) {
    //cout<<a<<" - "<<b<<" = "<<(a - b)<<endl;
    return (a - b);
}

int m_div(int a, int b) {
    //cout<<a<<" / "<<b<<" = "<<(a / b)<<endl;
    return (a / b);
}

//apply supplied operator op to operands a and b
int applyOper(char op, int a, int b) {
        switch (op) {
            case '+': return m_add(a, b);
            case '*': return m_mul(a, b);
            case '/': return m_div(a, b);
            case '-': return m_sub(a, b);
        }
    return 0;
}

int eval(string str) {
    int x, a, b;
    char op;
    Stack<int> vals;
    Stack<char> ops;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == '(' || str[i] == ' ') {
            continue;
        } else if (str[i] == '+' || str[i] == '*' || (str[i] == '-' && str[i+1] == ' ') || str[i] == '/' ) { 
            ops.push(str[i]);
        } else if (str[i] == ')') { 
            b = vals.pop();
            a = vals.pop();
            vals.push(applyOper(ops.pop(), a, b));
        } else if (isdigit(str[i]) || (str[i] == '-' && isdigit(str[i+1]))) {
            int e = i;
            while (isdigit(str[++e]));
            x = atoi(str.substr(i, e).c_str());
            vals.push(x);
            i = e-1;
        } 
    }
    while (!ops.empty()) {
        b = vals.pop();
        a = vals.pop();
        op = ops.pop();
        vals.push(applyOper(op, a, b));
    }
    return vals.pop();
}

#endif