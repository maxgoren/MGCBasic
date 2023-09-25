#ifndef mgcpl_parser_hpp
#define mgcpl_parser_hpp
#include <iostream>
#include "iterable_hashtable.hpp"
#include "mgcpl_globals.hpp"
using std::string;
using std::cout;
using std::endl;


IterableMap<string, string> valueMap;

TokenList* curr;
Token lookahead;
bool ready;

void error(TokenList* token) {
    cout<<"Error at line: "<<token->lineno<<" near "<<token->str<<endl;
}

void nexttoken() {
    if (curr != nullptr && curr->next != nullptr) {
        curr = curr->next;
        lookahead = curr->tok;
    } else {
        cout<<"Out of tokens."<<endl;
    }
}

bool match(Token token) {
    if (token == lookahead) {
        nexttoken();
        return 1;
    }
    return 0;
}

bool expect(Token t) {
    if (match(t))
        return true;
    error(curr);
    return false;
}

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