#ifndef mgcpl_lexer_hpp
#define mgcpl_lexer_hpp
#include <iostream>
#include "iterable_hashtable.hpp"
#include "mgcpl_globals.hpp"
using std::string;
using std::cout;
using std::endl;


class Lexer {
    private:
    IterableMap<string, Token> names_map;
    Token getToken(string word) {
        // single character tokens
        switch (word[0]) {
        case '(':
            return LPAREN;
        case ')':
            return RPAREN;
        case '\'':
            return QUOTESYM;
        case ';':
            return SEMICOLON;
        case '+':
            return ADD;
        case '-':
            return SUB;
        case '*':
            return MUL;
        case '/':
            return DIV;
        case ' ':
            return WHITESPACE;
        case '\t':
            return TAB;
        }
        if (word == "==")
            return EQ;
        if (word == ">")
            return GT;
        if (word == ">=")
            return GTEQ;
        if (word == "<")
            return LT;
        if (word == "<=")
            return LTEQ;
        if (word == "!=")
            return NOTEQ;
        if (word == ":=")
            return ASSIGNSYM;
        if (isdigit(word[0])) {
            return NUM;
        }
        if (word == "if")
            return IFSYM;
        if (word == "let")
            return LETSYM;
        if (word == "print")
            return PRINTSYM;
        if (word == "goto")
            return GOTO;
        if (word == "true")
            return TRUESYM;
        if (word == "false")
            return FALSESYM;
        if (word == "then")
            return THENSYM;
        if (word == "end")
            return ENDSYM;
        if (names_map.find(word) == names_map.end()) {
            names_map.put(word, IDSYM);
            return IDSYM;
        }
        else {
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