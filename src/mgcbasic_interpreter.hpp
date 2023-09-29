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

#ifndef mgcpl_interpreter_hpp
#define mgcpl_interpreter_hpp
#include <iostream>
#include <fstream>
#include "mgcbasic_parser_utils.hpp"
#include "mgcbasic_lexer.hpp"
#include "mgcbasic_eval.hpp"
#include "symboltable/hashtable.hpp"
#include "symboltable/avlmap.hpp"
using std::string;
using std::cout;
using std::endl;

class MGCBasic {
    private:
    Lexer lex;
    avlmap<int, TokenList*> program;
    avlmap<int, string> source;
    IterableMap<string, string> valueMap;
    /*void handleLet() {
        bool isExpression = false;
        if (match(LETSYM)) {
          if (matchToken(lookahead, IDSYM)) {
            string _id = curr->str;
            string _total = "";
            match(IDSYM);
            if (match(ASSIGNSYM)) {
                while (!matchToken(lookahead, SEMICOLON)) {
                    if (matchToken(lookahead, LPAREN)) {
                        _total += " ( ";
                    }
                    if (matchToken(lookahead, RPAREN)) {
                        _total += " ) ";
                    }
                    if (matchToken(lookahead, IDSYM)) {
                        if (valueMap.find(curr->str) == valueMap.end()) {
                            cout<<"Invalid identifier supplied on line: "<<curr->lineno<<", near: "<<curr->str<<endl;
                        } else {
                            _total += valueMap[curr->str];
                        }
                    } else if (matchToken(lookahead, NUM)) {
                        _total += curr->str;
                    } else if (matchToken(lookahead, ADD) || matchToken(lookahead, SUB) || matchToken(lookahead, MUL) || matchToken(lookahead, DIV)) {
                        isExpression = true;
                        _total += " " + curr->str + " ";
                    }
                    nexttoken();
                }
                if (isExpression) {
                    _total = to_string(eval(_total));
                }
                valueMap.put(_id, _total);
            }
          }
        }
    }*/

    void handleIdSym() {
        bool isExpression = false;
        if (matchToken(lookahead, IDSYM)) {
            string _id = curr->str;
            string _total = "";
            match(IDSYM);
            if (match(ASSIGNSYM)) {
                while (!matchToken(lookahead, SEMICOLON)) {
                    if (matchToken(lookahead, LPAREN)) {
                        _total += " ( ";
                    }
                    if (matchToken(lookahead, RPAREN)) {
                        _total += " ) ";
                    }
                    if (matchToken(lookahead, IDSYM)) {
                        if (valueMap.find(curr->str) == valueMap.end()) {
                            cout<<"Invalid identifier supplied on line: "<<curr->lineno<<", near: "<<curr->str<<endl;
                        } else {
                            _total += valueMap[curr->str] + " ";
                        }
                    } else if (matchToken(lookahead, NUM)) {
                        _total += curr->str + " ";
                    } else if (matchToken(lookahead, ADD) || matchToken(lookahead, SUB) || matchToken(lookahead, MUL) || matchToken(lookahead, DIV)) {
                        isExpression = true;
                        _total += curr->str + " ";
                    }
                    nexttoken();
                }
                if (isExpression) {
                    _total = to_string(eval(_total));
                }
                valueMap.put(_id, _total);
            }
        }
    }

    bool handleIf() {
        if (match(IFSYM)) {
            if (match(LPAREN)) {
                string firstVal, secondVal, relop;
                do {
                    if (matchToken(curr->tok, IDSYM))
                        firstVal += valueMap[curr->str];
                    if (matchToken(curr->tok, NUM))
                        firstVal += curr->str;
                    nexttoken();
                } while (!matchToken(lookahead, LT) && !matchToken(lookahead, GT) && !matchToken(lookahead, EQ) && !matchToken(lookahead, NOTEQ));
                relop = curr->str;
                nexttoken();
                do {
                    if (matchToken(curr->tok, IDSYM))
                        secondVal += valueMap[curr->str];
                    if (matchToken(curr->tok, NUM))
                        secondVal += curr->str;
                    nexttoken();
                } while (!matchToken(lookahead, RPAREN));
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
            if (matchToken(lookahead, QUOTESYM)) {
                nexttoken();
                string value;
                while (!matchToken(lookahead, QUOTESYM) && !matchToken(lookahead, SEMICOLON) && curr->next != nullptr) {
                    value += curr->str + " ";
                    nexttoken();
                }
                cout<<value<<endl;
            }
            if (matchToken(lookahead, IDSYM)) {
                string value;
                while (!matchToken(lookahead, SEMICOLON) && curr->next != nullptr) {
                    if (matchToken(lookahead, IDSYM))
                        value += valueMap[curr->str] + " ";
                    nexttoken();
                }
                cout<<value<<endl;
            }
        }
    }

    int handleFor(vector<TokenList*>& lines, int lp) {
        string varname;
        bool ready = false;
        int count;
        int finish;
        int stepping;
        if (match(FORSYM)) {
            if (matchToken(lookahead, IDSYM)) {
                varname = curr->str;
                valueMap[varname] = to_string(0);
                nexttoken();
                if (match(ASSIGNSYM)) {
                    if (matchToken(lookahead, NUM)) {
                        count = atoi(curr->str.c_str());
                        valueMap[varname] = curr->str;
                        nexttoken();
                        if (match(TOSYM)) {
                            if (matchToken(lookahead, NUM)) {
                                finish = atoi(curr->str.c_str());
                                nexttoken();
                            } else if (matchToken(lookahead, IDSYM)) {
                                if (valueMap.find(curr->str) != valueMap.end()) {
                                    finish = atoi(valueMap[curr->str].c_str());
                                    nexttoken();
                                } else {
                                    cout<<"Error: Unknown identifier: "<<curr->str<<endl;
                                }
                            } else {
                                cout<<"Error: cant step to "<<curr->str<<endl;
                            }
                            if (match(STEP)) {
                                stepping = atoi(curr->str.c_str());
                                ready = true;
                            } else {
                                cout<<"Syntex error at line: "<<curr->lineno<<", near: "<<curr->str<<endl;
                            }
                        } else {
                            cout<<"Expected TO, instead found "<<tokenNames[lookahead]<<endl;
                        }
                    }
                } else {
                    cout<<"Error: Expected assignment operator, instead found "<<tokenNames[lookahead]<<endl;
                }
            } else {
                cout<<"Error: unexpected keyword: "<<tokenNames[lookahead]<<endl;
            }
        } else {
            cout<<"Error: unexpected keyword: "<<tokenNames[lookahead]<<endl;
        }
        //something something code quality being
        //equally proportional to area under the curve
        //of nested statements.
        if (ready) {
            vector<TokenList*> scope;
            int sl = lp+1;
            while (!matchToken(lines[sl]->next->tok, NEXTSYM)) {
                scope.push_back(lines[sl++]);
            }
            while (count <= finish) {
                interpret(scope);
                count += stepping;
                valueMap[varname] = to_string(count);
            }
            return sl;
        }
        return 0;
    }

    void handleInput() {
        match(INPUT);
        if (matchToken(lookahead, IDSYM)) {
            string input;
            getline(cin, input);
            valueMap[curr->str] = input;
        } else {
            cout<<"Error: expected identifier on line: "<<curr->lineno<<", near "<<curr->str<<endl;
        }
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
        bool hasElse = false;
        for(int lp = 0; lp < lines.size(); lp++) {
            TokenList* lineStream = lines[lp];
            initparser(lineStream);
            if (matchToken(lookahead, NUM)) {
                nexttoken();
            } else {
                cout<<"Error: no line number supplied."<<endl;
            }
           
            switch(lookahead) {
                case IDSYM:
                    handleIdSym();
                    break;
                case LETSYM:
                    match(LETSYM);
                    handleIdSym();
                    break;
                case PRINTSYM:
                    handlePrint();
                    break;
                case INPUT:
                    handleInput();
                    break;
                case IFSYM:
                    result = handleIf();
                    if (result == false) {
                        nextLine = lp;
                        while (nextLine < lines.size() && lines[nextLine]->next->tok != ENDSYM) {
                            nextLine++;
                        }
                        lp = nextLine;
                    }
                    break;
                case FORSYM:
                    lp = handleFor(lines, lp);
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
    void readSourceFromFile(string filename) {
        program.clear();
        source.clear();
        ifstream ifile;
        ifile.open(filename, ios::in);
        if (!ifile.is_open()) {
            cout<<"Error: could not open "<<filename<<endl;
            exit(0);
        }
        string line;
        while (ifile.good()) {
            getline(ifile, line);
#if !(_MSC_VER) 
            line.pop_back();
#endif
            TokenList* tokenizedLine = lex.repl_tokenize(line);
            program.put(atoi(tokenizedLine->str.c_str()), tokenizedLine);
            source.put(atoi(tokenizedLine->str.c_str()), line);
        }
    }
    vector<TokenList*> prepForInterp() {
        vector<TokenList*> pg;
        for (auto t : program)
            pg.push_back(t.second);
        return pg;
    }
public:
    MGCBasic() {

    }
    void runProgram(vector<string>& program) {
        vector<TokenList*> tokenized = lex.analyze(program);
        interpret(tokenized);
    }

    void runFromFile(string filename) {
        readSourceFromFile(filename);
        vector<TokenList*> r2r = prepForInterp();
        interpret(r2r);
    }

    void REPL() {
        int autoline = 10;
        bool running = true;
        string inputline;
        
        while (running) {
            cout<<"repl> ";
            getline(cin, inputline);
            if (inputline == ".done" || inputline == ".quit" || inputline == ".exit")
                break;
            else if (inputline == ".run") {
                //I want to refactor the entire flow to eliminate the use of vectors, except for embedded use
                //I believe avlmap could be used for the whole enchillada. It would make GOTO cleaner, but IF harder..
                vector<TokenList*> r2r = prepForInterp();
                interpret(r2r);
            } else if (inputline == ".list") {
                for (auto l : source) {
                    cout<<l.second<<endl;
                }
            } else if (inputline.substr(0, 5) == ".load") {
                string filename = inputline.substr(6);
                readSourceFromFile(filename);
                cout<<filename<<": loaded."<<endl;
            } else if (inputline == ".symbols") {
                for (auto t : valueMap) {
                    cout<<t.first<<": "<<t.second<<endl;
                }
            } else if (inputline == ".tokens") {
                for (auto t : program) {
                    lex.showLexemes(t.second);
                }
            } else if (inputline == ".clear") {
                program.clear();
                source.clear();
                autoline = 10;
            } else if (inputline == ".keywords") {
                cout<<"Keywords: "<<endl;
                for (auto t : lex.getkeywords()) {
                    cout<<t.first<<": "<<tokenNames[t.second]<<endl;
                }
                break;            
            } else {
                if (!isdigit(inputline[0])) {
                    string nil = to_string(autoline) + " " + inputline;
                    inputline = nil;
                    autoline += 5;
                }
                TokenList* line = lex.repl_tokenize(inputline);
                program.put(atoi(line->str.c_str()), line);
                source.put(atoi(line->str.c_str()), inputline);
            }
        }
    }
};

#endif