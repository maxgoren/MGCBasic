#include <iostream>
#include <vector>
#include "mgcpl_interpreter.hpp"
using namespace std;


int main() {
    vector<string> program = {
                        "10 let x := 10 ;",
                        "20 let y := 5 ;",
                        "30 let z := x + y ;",
                        "40 if (z < 30) then",
                        "45   z := z + 1 ;",
                        "50   print z ; ",
                        "60   goto 40 ;",
                        "70 end ;",
                        "80 print 'nope' ;"
                    };
    MGCBasic mgc;
    mgc.runProgram(program);
    mgc.REPL();
    return 0;
}