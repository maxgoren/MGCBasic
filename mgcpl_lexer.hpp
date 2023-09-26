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
#include "hashtable.hpp"
#include "mgcpl_globals.hpp"
using std::string;
using std::cout;
using std::endl;


class Lexer {
    private:
    IterableMap<string, Token> vars_map;
    IterableMap<string, Token> keywords;
    void initKeywords() {    
        keywords["("] = LPAREN;   
        keywords[")"] = RPAREN;
        keywords["'"] = QUOTESYM;
        keywords[";"] = SEMICOLON;
        keywords["+"] = ADD;
        keywords["-"] = SUB;
        keywords["*"] = MUL;
        keywords["/"] = DIV;
        keywords[" "] = WHITESPACE;
        keywords["\t"] = TAB;
        keywords["=="] = EQ;
        keywords["!="] = NOTEQ;
        keywords[">"] = GT;
        keywords["<"] = LT;
        keywords[">="] = GTEQ;
        keywords["<="] = LTEQ;
        keywords[":="] = ASSIGNSYM;
        keywords["if"] = IFSYM;
        keywords["let"] = LETSYM;
        keywords["print"] = PRINTSYM;
        keywords["goto"] = GOTO;
        keywords["true"] = TRUESYM;
        keywords["false"] = FALSESYM;
        keywords["then"] = THENSYM;
        keywords["end"] = ENDSYM;
        keywords["input"] = INPUT;
    }
    Token getToken(string word) {
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
    vector<TokenList*> tokenize(vector<string>& program) {
        vector<TokenList*> lines;
        for (int lineno = 0; lineno < program.size(); lineno++) {
            string curr = program[lineno];
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
                    nt->lineno = lineno;
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
            lines.push_back(x);
        }
        return lines;
    }


    void printLexemes(TokenList *list) {
        for (auto token = list; token != nullptr; token = token->next) {
            cout << "<[" << endl;
            cout << " Token:   " << tokenNames[token->tok] << endl;
            cout << " line no: " << token->lineno << endl;
            cout << " index:   " << token->lpos << " - " << token->rpos << endl;
            cout << " String:  " << token->str << endl;
            cout << "]>" << endl;
        }
    }
public:
    Lexer() {
        initKeywords();
    }
    ~Lexer() {

    }
    
    vector<TokenList*> analyze(vector<string>& program) {
        return tokenize(program);

    }
    void showLexemes(TokenList* tokens) {
        printLexemes(tokens);
    }
    TokenList* repl_tokenize(string& curr) {
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
};

#endif