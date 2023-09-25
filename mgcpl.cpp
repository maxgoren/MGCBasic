#include <iostream>
#include <vector>
#include "mgcpl_interpreter.hpp"
using namespace std;




int main() {
    vector<string> program = {
                        "let x := 10 ;",
                        "let y := 5 ;",
                        "let z := x + y ;",
                        "if (z > 13) {",
                        "   print 'yes' ; ",
                        "}"
                    };
    MGCBasic mgc;
    mgc.runProgram(program);
    return 0;
}