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
    avlmap<int, TokenList*> m_program;
    avlmap<int, string> m_source;
    IterableMap<string, IterableMap<string,string>> m_valueMaps;
    IterableMap<Token, string> m_mathOpers;
    int autoline;
    bool replRunning;
    typedef pair<string, string> TypedValue;
    string getTypeFromVarname(string& varname) const;
    string& getValueFromVarname(string& varname);
    TypedValue handleStatement();
    void handleIdSym();
    bool handleIf();
    void handlePrint();
    void handlePrintln();
    int handleFor(vector<TokenList*>& lines, int lp);
    void handleDim();
    void handleInput();
    void handleREPLCode(string& line);
    void replPrompt(string& inputline);
    void replManager();
    void interpret(vector<TokenList*>& lines);
    bool readSourceFromFile(string filename);
    vector<TokenList*> prepForInterp();
    void clearProgram();
    void showSymbols();
    void showTokens();
    void displayCode();
    void displayKeywords();
public:
    MGCBasic();
    ~MGCBasic() { }
    void runProgram(vector<string>& program) {
        vector<TokenList*> tokenized = lex.analyze(program);
        interpret(tokenized);
    }
    void runFromFile(string filename) {
        if (readSourceFromFile(filename)) {
            vector<TokenList*> r2r = prepForInterp();
            interpret(r2r);
        }
    }
    void REPL() {
        replManager();
    }
};

template<> struct hashfn<Token> {
    size_t operator()(Token c) {
        return size_t(c);
    }
};

MGCBasic::MGCBasic() {
    m_mathOpers[MUL] = "*";
    m_mathOpers[ADD] = "+";
    m_mathOpers[SUB] = "-";
    m_mathOpers[DIV] = "/";
    m_mathOpers[SQUARED] = "**";
    m_valueMaps.put("num", IterableMap<string,string>());
    m_valueMaps.put("string", IterableMap<string,string>());
}

/// returns the type of supplied variable name
string MGCBasic::getTypeFromVarname(string& varname) const {
    for (auto maps : m_valueMaps) {
        if (maps.second.find(varname) != maps.second.end())
            return maps.first;
    }
    return "nf";
}

/// returns the value of supplied variable name
string& MGCBasic::getValueFromVarname(string& varname) {
    string valtype = getTypeFromVarname(varname);
    if (valtype == "nf") {
        cout<<"Error on line: "<<curr->lineno<<", unknown identifier: "<<varname<<endl;
    }
    return m_valueMaps[valtype][varname];
}


typename MGCBasic::TypedValue MGCBasic::handleStatement() {
    bool isExpression = false;
    string _total = "";
    string type_guess = "num";
    while (!matchToken(lookahead, SEMICOLON)) {
        if (matchToken(lookahead, LPAREN)) {
            _total += " ( ";
        } else if (matchToken(lookahead, RPAREN)) {
            _total += " ) ";
        } else if (matchToken(lookahead, NUM)) {
            _total += curr->str + " ";
            type_guess = "num";
        } else if (matchToken(lookahead, QUOTESYM)) {
            nexttoken();
            do {
                _total += curr->str + " ";
                nexttoken();
            } while (!matchToken(lookahead, QUOTESYM));
            break;
        } else if (matchToken(lookahead, IDSYM)) {
            if (getTypeFromVarname(curr->str) == "nf") {
                token_error(curr);
            } else {
                _total += getValueFromVarname(curr->str) + " ";
            }
        } else if (matchToken(lookahead, ADD) || matchToken(lookahead, MUL) ||
                    matchToken(lookahead, SUB) || matchToken(lookahead, DIV)) {
            isExpression = true;
            type_guess = "num";
            _total += curr->str + " ";
        } else {
            token_error(curr);
        }
        nexttoken();
    }
    if (isExpression) {
        _total = to_string(eval(_total));
    }
    return make_pair(type_guess, _total);
}

/// execute assignment statements
void MGCBasic::handleIdSym() {
    string type_guess = "num";
    if (matchToken(lookahead, IDSYM)) {
        string _id = curr->str;
        string _total = "";
        match(IDSYM);
        if (match(ASSIGNSYM)) {
            TypedValue result = handleStatement();
            type_guess = result.first;
            _total = result.second;
            m_valueMaps[type_guess].put(_id, _total);
        }
    }
}

/// Execute If statements
bool MGCBasic::handleIf() {
    bool isExpression = false;
    string firstVal, secondVal, relop, exprop;
    if (match(IFSYM)) {
        if (match(LPAREN)) {
            //get left sifde;
            do {
                if (matchToken(curr->tok, IDSYM))
                    firstVal += m_valueMaps[getTypeFromVarname(curr->str)][curr->str];
                if (matchToken(curr->tok, NUM))
                    firstVal += curr->str;
                nexttoken();
            } while (!matchToken(lookahead, RPAREN) && !matchToken(lookahead, LT) && !matchToken(lookahead, GT) && !matchToken(lookahead, EQ) && !matchToken(lookahead, NOTEQ) &&
                     !matchToken(lookahead, ADD) && !matchToken(lookahead, MUL) && !matchToken(lookahead, SUB) && !matchToken(lookahead, DIV));
            
            //relop, expression, or test for true?
            if (matchToken(lookahead, LT) || matchToken(lookahead, GT) || matchToken(lookahead, EQ) || matchToken(lookahead, NOTEQ)) {
                relop = curr->str;
                nexttoken();
            } else if (matchToken(lookahead, ADD) || matchToken(lookahead, MUL) || matchToken(lookahead, SUB) || matchToken(lookahead, DIV)) {
                isExpression = true;
                exprop = curr->str;
                nexttoken();
            } else if (matchToken(lookahead, RPAREN)) {
                return atoi(firstVal.c_str());
            }
            //get right side    
            do {
                if (matchToken(curr->tok, IDSYM))
                    secondVal += m_valueMaps[getTypeFromVarname(curr->str)][curr->str];
                if (matchToken(curr->tok, NUM))
                    secondVal += curr->str;
                nexttoken();
            } while (!matchToken(lookahead, RPAREN));
        } else {
            token_error(curr);
            cout<<"Syntax Error: Missing parentheses required."<<endl;
            return false;
        }
        if (isExpression) {
                return eval(firstVal + " " + exprop + " " + secondVal);
        } else {
            if (relop == "<")
                return (atoi(firstVal.c_str()) < atoi(secondVal.c_str()));
            if (relop == ">")
                return (atoi(firstVal.c_str()) > atoi(secondVal.c_str()));
            if (relop == "==")
                return (atoi(firstVal.c_str()) == atoi(secondVal.c_str()));
            if (relop == "!=")
                return (atoi(firstVal.c_str()) != atoi(secondVal.c_str()));
        } 
    }
    return false;
}

/// Execute print statemets
void MGCBasic::handlePrint() {
    string m_value;
    if (match(PRINTSYM) || match(PRINTLN)) {
        while (!matchToken(lookahead, SEMICOLON)) {
            if (matchToken(lookahead, NUM)) {
                m_value += curr->str + " ";
                nexttoken();
            } else if (matchToken(lookahead, QUOTESYM)) {
                nexttoken();
                while (curr) {
                    if (!matchToken(lookahead, COMMA))
                        m_value += curr->str + " ";
                    nexttoken();
                    if (matchToken(lookahead, QUOTESYM)) break;
                }
                nexttoken();
            } else if (matchToken(lookahead, IDSYM)) {
                m_value += m_valueMaps[getTypeFromVarname(curr->str)][curr->str] + " ";
                nexttoken();
            } else if (matchToken(lookahead, COMMA)) {
                nexttoken();
            }
            if (curr == nullptr || matchToken(lookahead, SEMICOLON)) { break; }
        }
    }
    cout<<m_value;
}

/// executes print statement and adds carriage return
void MGCBasic::handlePrintln() {
    handlePrint();
    cout<<endl;
}

/// Execute for loop
int MGCBasic::handleFor(vector<TokenList*>& lines, int lp) {
    string varname;
    bool ready = false;
    int count;
    int finish;
    int stepping;
    if (match(FORSYM)) {
        if (matchToken(lookahead, IDSYM)) {
            varname = curr->str;
            m_valueMaps["num"][varname] = to_string(0);
            nexttoken();
            if (match(ASSIGNSYM)) {
                if (matchToken(lookahead, NUM)) {
                    count = atoi(curr->str.c_str());
                    m_valueMaps["num"][varname] = curr->str;
                    nexttoken();
                    if (match(TOSYM)) {
                        if (matchToken(lookahead, NUM)) {
                            finish = atoi(curr->str.c_str());
                            nexttoken();
                        } else if (matchToken(lookahead, IDSYM)) {
                            if (m_valueMaps["num"].find(curr->str) != m_valueMaps["num"].end()) {
                                finish = atoi(m_valueMaps["num"][curr->str].c_str());
                                nexttoken();
                            } else {
                                token_error(curr);
                            }
                        } else {
                            cout<<"Error: cant step to "<<curr->str<<endl;
                        }
                        if (match(STEP)) {
                            stepping = atoi(curr->str.c_str());
                            ready = true;
                        } else token_error(curr);
                    } else token_error(curr);  // Standing here it occurs to me
                } else token_error(curr);     //    (_)
            } else token_error(curr);        // -\_(-o-)-\_
        } else token_error(curr);           //     _| |_
    } else token_error(curr);              // nevermind.
    
    //gather the lines from inside the loop block
    //and recursively call interpret on that piece
    //finish number of times.
    if (ready) {
        vector<TokenList*> scope;
        int sl = lp+1;
        while (!(matchToken(lines[sl]->next->tok, NEXTSYM) && varname == lines[sl]->next->next->str)) {
            scope.push_back(lines[sl++]);
        }
        while (count <= finish) {
            interpret(scope);
            count += stepping;
            m_valueMaps["num"][varname] = to_string(count);
        }
        return sl;
    }
    return 0;
}

/// Handles the declaring of variable names
void MGCBasic::handleDim() {
    string identifier, valuetype;
    if (match(DIM)) {
        if (matchToken(lookahead, IDSYM)) {
            identifier = curr->str;
            nexttoken();
            if (match(AS)) {
                valuetype = curr->str;
                //I know this is overkill for a 2-type type system
                //conversely it makes _adding_ more types a breeze
                if (m_valueMaps.find(valuetype) == m_valueMaps.end()) {
                    cout<<"Error: uknown type: "<<valuetype<<endl;
                    return;
                } else {
                    m_valueMaps[valuetype][identifier] = "";
                    return;
                }
            } else {
                token_error(curr);
                return;
            }
        } else {
            token_error(curr);
        }
    }
}

/// retrieve input from the user
void MGCBasic::handleInput() {
    match(INPUT);
    if (matchToken(lookahead, IDSYM)) {
        string input;
        getline(cin, input);
        m_valueMaps[getTypeFromVarname(curr->str)][curr->str] = input;
    } else {
        token_error(curr);
    }
}

/// display all variable names and their values for the currently loaded program
void MGCBasic::showSymbols() {
    cout<<"Currently Assigned Variables: \n";
    for (auto maps : m_valueMaps) {
        cout<<maps.first<<" types: "<<endl;
        for (auto symbols : maps.second)
            cout<<" --> "<<symbols.first<<": "<<symbols.second<<endl;
    }
}

/// show the tokenized version of currently loaded program
void MGCBasic::showTokens() {
    for (auto line : m_program) {
        lex.showLexemes(line.second);
    }
}

/// clears currently loaded program resetting the environment
void MGCBasic::clearProgram() {
    if (!m_program.empty()) {
        m_program.clear();
        m_source.clear();
        m_valueMaps["num"].clear();
        m_valueMaps["string"].clear();
    }
    autoline = 10;
}

/// display prompt, retrieve input
void MGCBasic::replPrompt(string& inputline) {
    if (matchToken(lookbehind, PRINTSYM)) cout<<"\n";
    cout<<m_PROMPT<<"> ";
    getline(cin, inputline);
}

void MGCBasic::replManager() {
    autoline = 10;
    replRunning = true;
    string inputline;
    while (replRunning) {
        replPrompt(inputline);
        if (inputline == ".done" 
        || inputline == ".quit")            break;
        else if (inputline == ".list")      displayCode();
        else if (inputline == ".symbols")   showSymbols();
        else if (inputline == ".tokens")    showTokens();
        else if (inputline == ".clear")     clearProgram();
        else if (inputline == ".keywords")  displayKeywords();
        else if (inputline.substr(0, 5) == ".load") 
            readSourceFromFile(inputline.substr(6));
        else if (inputline == ".run") {
            vector<TokenList*> r2r = prepForInterp();
            interpret(r2r);
        }            
        else handleREPLCode(inputline);
    }
}

void MGCBasic::handleREPLCode(string& inputline) {
    if (!isdigit(inputline[0])) {
        string nil = to_string(autoline) + " " + inputline;
        inputline = nil;
        autoline += 5;
    }
    TokenList* line = lex.repl_tokenize(inputline);
    m_program.put(atoi(line->str.c_str()), line);
    m_source.put(atoi(line->str.c_str()), inputline);
}

void MGCBasic::interpret(vector<TokenList*>& lines) {
    bool stepping = false;
    int nextLine = 0;
    bool result = false;
    bool hasElse = false;
    for(int lp = 0; lp < lines.size(); lp++) {
        TokenList* lineStream = lines[lp];
        if (lp > 0) lookbehind = lines[lp-1]->tok;
        initparser(lineStream);
        if (matchToken(lookahead, NUM)) {
            nexttoken();
        } else {
            cout<<"Error: no line number supplied."<<endl;
        }
        switch(lookahead) {
            case DIM:
                handleDim();
                break;
            case IDSYM:
                handleIdSym();
                break;
            case LETSYM:
                match(LETSYM);
                handleIdSym();
                break;
            case PRINTLN:
                handlePrintln();
                break;
            case PRINTSYM:
                handlePrint();
                break;
            case INPUT:
                handleInput();
                break;
            case FORSYM:
                lp = handleFor(lines, lp);
                break;
            case IFSYM:
                result = handleIf();
                if (result == false) {
                    nextLine = lp;
                    while (nextLine < lines.size() && !matchToken(lines[nextLine]->next->tok, ENDSYM)) {
                        nextLine++;
                    }
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

bool MGCBasic::readSourceFromFile(string filename) {
    m_program.clear();
    m_source.clear();
    ifstream ifile;
    ifile.open(filename, ios::in);
    if (!ifile.is_open()) {
        cout<<"Error: could not open "<<filename<<endl;
        return false;
    }
    string line;
    while (ifile.good()) {
        getline(ifile, line);
#if !(_MSC_VER) 
        line.pop_back();
#endif
        TokenList* tokenizedLine = lex.repl_tokenize(line);
        m_program.put(atoi(tokenizedLine->str.c_str()), tokenizedLine);
        m_source.put(atoi(tokenizedLine->str.c_str()), line);
    }
    cout<<filename<<": loaded."<<endl;
    return true;
}

vector<TokenList*> MGCBasic::prepForInterp() {
    vector<TokenList*> pg;
    for (auto line : m_program)
        pg.push_back(line.second);
    return pg;
}

void MGCBasic::displayCode() {
    if (m_source.empty()) return;
    for (auto line : m_source) {
        cout<<line.second<<endl;
    }
}

void MGCBasic::displayKeywords() {
    cout<<"Keywords: "<<endl;
    for (auto keyword : lex.getkeywords()) {
        if (keyword.first.length() > 1) {
            cout<<keyword.first<<": "<<tokenNames[keyword.second]<<endl;
        }
    }
}

#endif