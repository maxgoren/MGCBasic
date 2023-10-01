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
#ifndef mgcpl_lexer_hpp
#define mgcpl_lexer_hpp
#include <iostream>
#include "symboltable/hashtable.hpp"
#include "mgcbasic_globals.hpp"
using std::string;
using std::cout;
using std::endl;


class Lexer {
    private:
    IterableMap<string, Token> vars_map;
    IterableMap<string, Token> keywords;
    Token getToken(string word);
    vector<TokenList*> tokenize(vector<string>& program);
    void printLexemes(TokenList *list);
    void initKeywords();
public:
    Lexer() {
        initKeywords();
    }
    ~Lexer() { }
    vector<TokenList*> analyze(vector<string>& program) {
        return tokenize(program);

    }
    void showLexemes(TokenList* tokens) {
        printLexemes(tokens);
    }
    IterableMap<string, Token>& getkeywords() {
        return keywords;
    }
    TokenList* repl_tokenize(string& curr);
};

//initialize keywords for recognizing syntax elements and identifiers 
void Lexer::initKeywords() {    
    keywords["("] = LPAREN;   
    keywords[")"] = RPAREN;
    keywords["'"] = QUOTESYM;
    keywords[";"] = SEMICOLON;
    keywords["+"] = ADD;
    keywords["-"] = SUB;
    keywords["*"] = MUL;
    keywords["/"] = DIV;
    keywords["**"] = SQUARED;
    keywords[" "] = WHITESPACE;
    keywords["\t"] = TAB;
    keywords["=="] = EQ;
    keywords["!="] = NOTEQ;
    keywords[">"] = GT;
    keywords["<"] = LT;
    keywords[">="] = GTEQ;
    keywords["<="] = LTEQ;
    keywords[":="] = ASSIGNSYM;
    keywords[","] = COMMA;
    keywords["let"] = LETSYM;
    keywords["print"] = PRINTSYM;
    keywords["println"] = PRINTLN;
    keywords["input"] = INPUT;
    keywords["goto"] = GOTO;
    keywords["for"] = FORSYM;
    keywords["next"] = NEXTSYM;
    keywords["to"] = TOSYM;
    keywords["step"] = STEP;
    keywords["if"] = IFSYM;
    keywords["true"] = TRUESYM;
    keywords["false"] = FALSESYM;
    keywords["then"] = THENSYM;
    keywords["end"] = ENDSYM;
    keywords["dim"] = DIM;
    keywords["as"] = AS;
    keywords["num"] = NUM;
    keywords["string"] = STRING;
}

/// Retrieves appropriate token for provided symbol
Token Lexer::getToken(string word) {
    if (word.size() >= 2) {
        if (word[0] == '-' && isdigit(word[1])) {
            return NUM;
        }
    }
    if (isdigit(word[0])) {
        return NUM;
    }
    if (keywords.find(word) != keywords.end())
        return keywords[word];
    if (vars_map.find(word) == vars_map.end()) {
        vars_map.put(word, IDSYM);
        return IDSYM;
    } else {
        return IDSYM;
    }
    return ERROR;
}

//tokenize entire program supplied as vector of strings
vector<TokenList*> Lexer::tokenize(vector<string>& program) {
    vector<TokenList*> lines;
    for (int lineno = 0; lineno < program.size(); lineno++) {
        lines.push_back(repl_tokenize(program[lineno]));
    }
    return lines;
}

//print token stream to output
void Lexer::printLexemes(TokenList *list) {
    for (auto token = list; token != nullptr; token = token->next) {
        cout << "<[";
        cout << " Token:   " << tokenNames[token->tok] << ", ";
        cout << " line no: " << token->lineno << ", ";
        cout << " index:   " << token->lpos << " - " << token->rpos << ", ";
        cout << " String:  " << token->str;
        cout << "]>" << endl;
    }
}

/// tokenize single line of code
TokenList* Lexer::repl_tokenize(string& curr) {
    TokenList head;
    TokenList *lst = &head;
    for (int i = 0; i < curr.size(); i++) {
        int lpos = i;
        int rpos = i;
        while (rpos < curr.length() && curr[rpos++] != ' ') {
            if (curr[rpos - 1] == '(' || curr[rpos - 1] == '\'')
                break;
            if (curr[rpos] == '\'' || curr[rpos] == ')')
                break;
            if (curr[rpos] == ';' || curr[rpos] == ',')
                break;
        }
        if (curr[rpos - 1] == ' ') {
            rpos--;
            i = rpos;
        }
        else
            i = rpos - 1;
        string piece = curr.substr(lpos, rpos - lpos);
        if (piece.size() > 0) {
            Token tok = getToken(piece);
            TokenList *nt = new TokenList;
            lst->next = nt;
            nt->str = piece;
            nt->lineno = atoi(head.next->str.c_str());
            nt->lpos = lpos;
            nt->rpos = rpos;
            nt->tok = tok;
            nt->str = piece;
            nt->next = nullptr;
            lst->next = nt;
            lst = nt;
        }
    }
    auto x = head.next;
    return x;
}

#endif