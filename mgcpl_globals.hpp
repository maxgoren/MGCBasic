#ifndef GLOBALS_HPP
#define GLOBALS_HPP
#include <vector>
#include <string>
using namespace std;

enum Token {
    ERROR, LPAREN, RPAREN, THEN, END, LETSYM, IFSYM, PRINTSYM, 
    NUM, ADD, SUB, MUL, DIV, EQ, NOTEQ, GT, LT, LTEQ, GTEQ, TAB, WHITESPACE,
    QUOTESYM, SEMICOLON, IDSYM, ASSIGNSYM, TRUESYM, FALSESYM, FORSYM, NEXTSYM, GOTO
};

vector<string> tokenNames = {
    "error", "LPAREN", "RPAREN", "THEN", "END", "LETSYM", "IFSYM", "PRINTSYM", 
    "NUM", "ADD", "SUB", "MUL", "DIV", "EQ", "NOTEQ", "GT", "LT", "LTEQ", "GTEQ", "TAB", "WHITESPACE",
    "QUOTESYM", "SEMICOLON", "IDSYM", "ASSIGNSYM", "TRUESYM", "FALSESYM", "FORSYM", "NEXTSYM", "GOTO"};

struct TokenList {
    Token tok;
    string str;
    int lineno;
    int lpos;
    int rpos;
    TokenList* next;
};

#endif