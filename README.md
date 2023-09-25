# MGCBasic
A *very* basic BASIC interpreter

As it stands, code should be passed to the interpreter as a vector of strings.
I told you this was *very* basic.

example program:
      
      #include <iostream>
      #include <vector>
      #include "mgcpl_interpreter.hpp"
      using namespace std;

      int main() {
          vector<string> program = {
                        "let x := 10 ;",
                        "let y := 5 ;",
                        "let z := x + y ;",
                        "if (z > 13) then",
                        "   print 'yes' ; ",
                        "end"
                    };
           MGCBasic mgc;
           mgc.runProgram(program);
           return 0;
     }
