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
#include <algorithm>
#include "mgcbasic_parser_utils.hpp"
#include "mgcbasic_lexer.hpp"
#include "mgcbasic_eval.hpp"
#include "symboltable/hashtable.hpp"
#include "symboltable/hashset.hpp"
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
    IterableSet<Token> m_mathOpers;
    IterableSet<Token> m_relOpers;
    int autoline;
    bool replRunning;
    typedef pair<string, string> TypedValue;
    string getTypeFromVarname(string& varname) const;
    string& getValueFromVarname(string& varname);
    bool assignDataToVarname(string& varname, string& data, string& value_type);
    bool isString();
    bool isExpression();
    TypedValue handleStatement();
    string handleExpression();
    string handleString();
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
    void saveCodeToFile(string filename);
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

MGCBasic::MGCBasic() {
    m_mathOpers.put(MUL);  m_relOpers.put(LT);
    m_mathOpers.put(ADD);  m_relOpers.put(GT);
    m_mathOpers.put(SUB);  m_relOpers.put(EQ);
    m_mathOpers.put(DIV);  m_relOpers.put(NOTEQ);
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

bool MGCBasic::assignDataToVarname(string& _id, string& _data, string& type_guess) {
    if (lex.getkeywords().find(_id) != lex.getkeywords().end()) {
        token_error(curr);
        cout<<"Syntax Error: "<<_id<<" is a reserved keyword!"<<endl;
        return false;
    }
    if (isdigit(_id[0])) {
        token_error(curr);
        cout<<"Syntax Error: variable names must start with a letter"<<endl;
        return false;
    }
    m_valueMaps[type_guess].put(_id, _data);
    return true;
}

//you can do O(n) operations when N is < 10 till the cows come home. 
//Were parsing lines of BASIC afterall
bool MGCBasic::isString() {
    for (TokenList* t = curr; t != nullptr; t = t->next)
        if (matchToken(t->tok, QUOTESYM))
            return true;
        else if (matchToken(t->tok, IDSYM) && getTypeFromVarname(t->str) == "string")
            return true;
    return false;
}

bool MGCBasic::isExpression() {
    for (TokenList* t = curr; t != nullptr; t = t->next)
        if (m_relOpers.find(t->tok) != m_relOpers.end() || m_mathOpers.find(t->tok) != m_mathOpers.end())
            return true;
    return false;
}

string MGCBasic::handleString() {
    string _total = "";
    if (match(QUOTESYM)) {
            nexttoken();
            do {
                _total += curr->str + " ";
                nexttoken();
            } while (!matchToken(lookahead, QUOTESYM));
    }
    return _total;  
}

string MGCBasic::handleExpression() {
    string _total = "";
    int parCount = 1;
    do {
        if (matchToken(lookahead, LPAREN)) {
            parCount++;
            _total += curr->str;
        }
        if (matchToken(lookahead, RPAREN) && parCount > 1) {
            _total += curr->str;
            parCount--;
        }
        if (matchToken(lookahead, IDSYM))
         if (getTypeFromVarname(curr->str) == "nf") {
                token_error(curr);
            } else {
                _total += getValueFromVarname(curr->str) + " ";
            }
        if (matchToken(lookahead, NUM))
            _total += curr->str + " ";
        if (m_mathOpers.find(lookahead) != m_mathOpers.end() || m_relOpers.find(lookahead) != m_relOpers.end())
            _total += curr->str + " ";
        if (matchToken(lookahead, SEMICOLON) || matchToken(lookahead, THENSYM))
            break;
        nexttoken();
    } while (!matchToken(lookahead, RPAREN) && parCount > 0);
    cout<<_total<<endl;
    return _total;
}

typename MGCBasic::TypedValue MGCBasic::handleStatement() {
    string _total = "";
    string type_guess = "num";
    if (isExpression())
        return make_pair(type_guess, to_string(eval(handleExpression())));
    if (isString())
        return make_pair("string", handleString());
    while (!matchToken(lookahead, SEMICOLON) && !matchToken(lookahead, THENSYM)) {
        if (matchToken(lookahead, LPAREN)) {
            _total += " ( ";
        } else if (matchToken(lookahead, RPAREN)) {
            _total += " ) ";
        } else if (matchToken(lookahead, NUM)) {
            _total += curr->str + " ";
            type_guess = "num";
        } else if (matchToken(lookahead, QUOTESYM)) {
            _total = handleString();
        } else if (matchToken(lookahead, IDSYM)) {
            if (getTypeFromVarname(curr->str) == "nf") {
                token_error(curr);
            } else {
                _total += getValueFromVarname(curr->str) + " ";
            }
        } else if (m_mathOpers.find(lookahead) != m_mathOpers.end()) {
            _total += curr->str + " ";
        } else {
            token_error(curr);
        }
        nexttoken();
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
            assignDataToVarname(_id, _total, type_guess);
        }
    }
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


/// Execute If statements
bool MGCBasic::handleIf() {
    bool isExpression = false;
    int parCount = 0;
    string _total;
    if (match(IFSYM)) {
        if (match(LPAREN)) {
            auto tmp = handleStatement();
            return eval(tmp.second);
        } else {
            token_error(curr);
            cout<<"Syntax Error: Missing parentheses required."<<endl;
            return false;
        }
    }
    return false;
}

/// Execute print statemets
void MGCBasic::handlePrint() {
    string m_value;
    bool isExpression = false;
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
            } else if (m_mathOpers.find(lookahead) != m_mathOpers.end()) {
                isExpression = true;
                m_value += curr->str + " ";
                nexttoken();
            }
            if (curr == nullptr || matchToken(lookahead, SEMICOLON)) { break; }
        }
    }
    if (isExpression) {
        m_value = eval(m_value);
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
    //and call interpret on that section N times
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
            if (!m_program.empty()) {
                vector<TokenList*> r2r = prepForInterp();
                interpret(r2r);
            }
        } else if (inputline.substr(0, 5) == ".save") {
            saveCodeToFile(inputline.substr(6));
        } else handleREPLCode(inputline);
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
        if (matchToken(lookahead, NUM) && curr->next == nullptr)
            continue;
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
    while (!ifile.eof()) {
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

void MGCBasic::saveCodeToFile(string filename) {
    ofstream ofile;
    ofile.open(filename, ios::out);
    if (ofile.is_open()) {
        for (auto line : m_source) {
            ofile << line.second << "\r\n";
        }
        ofile.close();
        cout<<filename<<" saved!"<<endl;
    } else {
        cout<<"Error: Couldn't open "<<filename<<" for writing!"<<endl;
    }
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