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



struct node {
    char op;
    int value;
    node* left;
    node* right;
    node(char op_) {
        op = op_;
        left = nullptr;
        right = nullptr;
    }
    node(int value_) {
        value = value_;
        left = nullptr;
        right = nullptr;
    }
};

//Dijkstra's shungting yard algorithm
//converts infix expression to postfix
string infix2postfix(string expr) {
    Stack<char> sf;
    string postfix;
    for (int i = 0; i < expr.length(); i++) {
        if (expr[i] == ')') postfix.push_back(sf.pop());
        else if (expr[i] == '+' || expr[i] == '*' || (i < expr.size() - 1 && expr[i] == '-' && !isdigit(expr[i+1])) || expr[i] == '/' )
            sf.push(expr[i]);
        else {
            int e = i;
            if (expr[e] == '-') postfix.push_back('-');
            while (isdigit(expr[e])) {
                postfix.push_back(expr[e++]);
            }
            if (e != i)
                postfix.push_back(' ');
            i = e;
        }
    }
    while (!sf.empty()) {
        postfix.push_back(sf.pop());
        postfix.push_back(' ');
    }
    return postfix;
}

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

//is this node a leaf node?
bool isLeaf(node* x) {
    if (x == nullptr) return true;
    return x->left == nullptr && x->right == nullptr;
}


//walk the tree postorder, evaluating the expression
//and freeing the memory used by the tree.
int evalTree(node* x) {
    if (x == nullptr) 
        return 0;

    if (x && isLeaf(x))
        return x->value;
    int lval = evalTree(x->left);
    int rval = evalTree(x->right);
    int res = applyOper(x->op, lval, rval);
    return res;
}

void cleanup(node* x) {
    if (x != nullptr) {
        cleanup(x->left);
        cleanup(x->right);
        delete x;
    }
}

//build binary expression tree 
//bottom up from postfix expression
node* buildExpressionTree(string expr) {
    node* x;
    Stack<node*> sf;
    for (int i = 0; i < expr.size(); i++) {
        if (expr[i] == '+' || expr[i] == '*' ||expr[i] == '-' || expr[i] == '/' ) {
            x = new node(expr[i]);
            x->right = sf.pop();
            x->left = sf.pop();
            sf.push(x);
        }
        if (isdigit(expr[i]) || (i < expr.size() - 1 && expr[i] == '-' && isdigit(expr[i+1]))) {
            int e = i;
            if (i < expr.size() - 1 && expr[i] == '-' && isdigit(expr[i+1])) e++;
            while (isdigit(expr[++e]));
            sf.push(new node(atoi(expr.substr(i, e).c_str())));
            i = e;
        }
    }
    return x;
}
 
//evaluate a given infix expression by 
//converting it to postfix, building a
//bottom up expression tree, and walking
//the tree
int eval(string expr) {
    //cout<<"------------------------"<<endl;
    //cout<<"infix: "<<expr<<endl;
    expr = infix2postfix(expr);
    node* x = buildExpressionTree(expr);
    //cout<<"eval: "<<expr<<endl;
    int result = evalTree(x);
    cleanup(x);
    //cout<<"Result: "<<result<<endl;
    return result;
}

#endif