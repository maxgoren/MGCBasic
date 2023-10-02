/*
MIT License

Copyright (c) 2023 Max Goren, http://www.maxgcoding.com

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
#ifndef GLOBALS_HPP
#define GLOBALS_HPP
#include <vector>
#include <string>
#include "symboltable/hashfn.hpp"
using namespace std;

enum Token {
    ERROR, LPAREN, RPAREN, THENSYM, ENDSYM, LETSYM, IFSYM, PRINTSYM, 
    NUM, ADD, SUB, MUL, DIV, EQ, NOTEQ, GT, LT, LTEQ, GTEQ, TAB, WHITESPACE,
    QUOTESYM, SEMICOLON, IDSYM, ASSIGNSYM, TRUESYM, FALSESYM, FORSYM, NEXTSYM, 
    GOTO, INPUT, ELSESYM, TOSYM, STEP, DIM, AS, STRING, COMMA, PRINTLN, SQUARED
};

template<> struct hashfn<Token> {
    size_t operator()(Token c) {
        return size_t(c);
    }
};

vector<string> tokenNames = {
    "-error-", "LPAREN", "RPAREN", "THENSYM", "ENDSYM", "LETSYM", "IFSYM", "PRINTSYM", 
    "NUM", "ADD", "SUB", "MUL", "DIV", "EQ", "NOTEQ", "GT", "LT", "LTEQ", "GTEQ", "TAB", 
    "WHITESPACE", "QUOTESYM", "SEMICOLON", "IDSYM", "ASSIGNSYM", "TRUESYM", "FALSESYM", 
    "FORSYM", "NEXTSYM", "GOTO", "INPUT", "ELSESYM", "TOSYM", "STEP", "DIM", "AS", 
    "STRING", "COMMA", "PRINTLN", "SQUARED" };

const string m_PROMPT = "mgcb";
const string m_NAME = "MGCBasic";
const string m_VERSION = "0.4b";

#endif