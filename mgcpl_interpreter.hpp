#ifndef mgcpl_interpreter_hpp
#define mgcpl_interpreter_hpp
#include <iostream>
#include "mgcpl_parser.hpp"
#include "mgcpl_lexer.hpp"
#include "eval_expr.hpp"
#include "iterable_hashtable.hpp"
using std::string;
using std::cout;
using std::endl;

class MGCBasic {
    private:
    void handleLet() {
        bool isExpression;
        if (match(LETSYM)) {
          if (lookahead == IDSYM) {
            string _id = curr->str;
            string _total = "";
            match(IDSYM);
            if (match(ASSIGNSYM)) {
                while (lookahead != SEMICOLON) {
                    if (lookahead == IDSYM) {
                        if (valueMap.find(curr->str) == valueMap.end()) {
                            cout<<"Invalid identifier supplied on line: "<<curr->lineno<<", near: "<<curr->str<<endl;
                        } else {
                            _total += valueMap[curr->str];
                        }
                    } else if (lookahead == NUM) {
                        _total += curr->str;
                    } else if (lookahead == ADD || lookahead == SUB || lookahead == MUL || lookahead == DIV) {
                        isExpression = true;
                        _total += curr->str;
                    }
                    nexttoken();
                }
                if (isExpression) {
                    //cout<<"eval: "<<_total<<endl;
                    _total = to_string(eval(_total));
                    //cout<<"Eval'd to: "<<_total<<endl;
                }
                valueMap.put(_id, _total);
                //cout<<"Assigned: "<<_id<<" = "<<valueMap[_id]<<endl;
            }
          }
        }
    }

    void handleIdSym() {
        bool isExpression;
        if (lookahead == IDSYM) {
            string _id = curr->str;
            string _total = "";
            match(IDSYM);
            if (match(ASSIGNSYM)) {
                while (lookahead != SEMICOLON) {
                    if (lookahead == IDSYM) {
                        if (valueMap.find(curr->str) == valueMap.end()) {
                            cout<<"Invalid identifier supplied on line: "<<curr->lineno<<", near: "<<curr->str<<endl;
                        } else {
                            _total += valueMap[curr->str];
                        }
                    } else if (lookahead == NUM) {
                        _total += curr->str;
                    } else if (lookahead == ADD || lookahead == SUB || lookahead == MUL || lookahead == DIV) {
                        isExpression = true;
                        _total += curr->str;
                    }
                    nexttoken();
                }
                if (isExpression) {
                    //cout<<"eval: "<<_total<<endl;
                    _total = to_string(eval(_total));
                    //cout<<"Eval'd to: "<<_total<<endl;
                }
                valueMap.put(_id, _total);
                //cout<<"Assigned: "<<_id<<" = "<<valueMap[_id]<<endl;
            }
        }
    }

    bool handleIf() {
        if (match(IFSYM)) {
            if (match(LPAREN)) {
                string firstVal, secondVal, relop;
                do {
                    if (curr->tok == IDSYM)
                        firstVal += valueMap[curr->str];
                    if (curr->tok == NUM)
                        firstVal += curr->str;
                    nexttoken();
                } while (lookahead != LT && lookahead != GT && lookahead != EQ && lookahead != NOTEQ);
                relop = curr->str;
                nexttoken();
                do {
                    if (curr->tok == IDSYM)
                        secondVal += valueMap[curr->str];
                    if (curr->tok == NUM)
                        secondVal += curr->str;
                    nexttoken();
                } while (lookahead != RPAREN);
                if (relop == "<")
                    return (atoi(firstVal.c_str()) < atoi(secondVal.c_str()));
                if (relop == ">")
                    return (atoi(firstVal.c_str()) > atoi(secondVal.c_str()));
                if (relop == "==")
                    return (atoi(firstVal.c_str()) == atoi(secondVal.c_str()));
                if (relop == "!=")
                    return (atoi(firstVal.c_str()) != atoi(secondVal.c_str()));
            } else {
                cout<<"Syntax error: missing '(' on line: "<<curr->lineno<<endl;
                return false;
            }
        }
        return false;
    }

    void handlePrint() {
        if (match(PRINTSYM)) {
            if (lookahead == QUOTESYM) {
                nexttoken();
                string value;
                while (lookahead != QUOTESYM) {
                    value += curr->str;
                    nexttoken();
                }
                cout<<value<<endl;
            }
            if (lookahead == IDSYM) {
                string value;
                while (lookahead != SEMICOLON) {
                    if (lookahead == IDSYM)
                        value += valueMap[curr->str] + " ";
                    nexttoken();
                }
                cout<<value<<endl;
            }
        }
    }

    int handleGoto(int lp) {
        return lp;
    }

    void printLineTokens(TokenList* st) {
        for (auto t = st; t != nullptr; t = t->next) {
            cout<<tokenNames[t->tok]<<"("<<t->str<<") -> ";
        }
        cout<<endl;
    }

    void interpret(vector<TokenList*>& lines) {
        bool stepping = false;
        int nextLine = 0;
        bool result = false;
        int ip = 0; //instruction pointer;
        for(int lp = 0; lp < lines.size(); lp++) {
            TokenList* lineStream = lines[lp];
            initparser(lineStream);
            if (lookahead == NUM) {
                nexttoken();
            } else {
                cout<<"Error: no line number supplied."<<endl;
            }
           
            switch(lookahead) {
                case IDSYM:
                    handleIdSym();
                    break;
                case LETSYM:
                    handleLet();
                    break;
                case PRINTSYM:
                    handlePrint();
                    break;
                case IFSYM:
                    result = handleIf();
                    //cout<<"Handle if: "<<((result) ? "true":"false")<<endl;
                    if (result == false) {
                        nextLine = lp;
                        while (nextLine < lines.size() && lines[nextLine]->next->tok != ENDSYM) nextLine++;
                        lp = nextLine;
                    }
                    break;
                case GOTO:
                    nexttoken();
                    for (int i = 0; i < lines.size(); i++) {
                        if (lines[i]->str == curr->str) {
                            lp = i-1;
                            break;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
public:
    MGCBasic() {

    }
    void runProgram(vector<string>& program) {
        Lexer lex;
        vector<TokenList*> tokenized = lex.analyze(program);
        interpret(tokenized);
    }

    void REPL() {
        Lexer lex;
        bool running = true;
        string inputline;
        vector<TokenList*> program;
        vector<string> source;
        while (running) {
            cout<<"> ";
            getline(cin, inputline);
            if (inputline == ".done")
                break;
            else if (inputline == ".run") {
                interpret(program);
            } else if (inputline == ".list") {
                for (string l : source) {
                    cout<<l<<endl;
                }
            } else {
                TokenList* line = lex.repl_tokenize(inputline);
                if (line->tok != NUM) {
                    cout<<"Error: must inclue line number."<<endl;
                } else {
                    program.push_back(line);
                    source.push_back(inputline);
                }
            }
        }
    }
};

#endif