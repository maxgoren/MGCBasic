#ifndef mgcpl_parser_hpp
#define mgcpl_parser_hpp
#include <iostream>
#include "mgcbasic_globals.hpp"
using std::string;
using std::cout;
using std::endl;


/// in memory representation of tokenized source
struct TokenList {
    Token tok;
    string str;
    int lineno;
    int lpos;
    int rpos;
    TokenList* next;
};

/// parser globals
TokenList* curr;
Token lookahead;
Token lookbehind;
bool ready;

/// display error message
void token_error(TokenList* token) {
    cout<<"Error at line: "<<token->lineno<<" near "<<token->str<<endl;
}

/// advannce stream forward
void nexttoken() {
    if (curr != nullptr && curr->next != nullptr) {
        curr = curr->next;
        lookahead = curr->tok;
    } else {
        cout<<"Out of tokens."<<endl;
        exit(0);
    }
}

/// Match and consume
bool match(Token token) {
    if (token == lookahead) {
        nexttoken();
        return 1;
    }
    return 0;
}
/// Match without consuming
bool matchToken(Token a, Token b) {
    return a == b;
}

/// initialize parser for token stream
bool initparser(TokenList* stream) {
    if (stream != nullptr) {
        curr = stream;
        lookahead = curr->tok;
        ready = true;
    } else {
        cout<<"Error: Bad token stream"<<endl;
        ready = false;
    }
    return ready;
}

#endif