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
                        "10 let x := 10 ;",
                        "20 let y := 5 ;",
                        "30 let z := x + y ;",
                        "40 if (z < 21) then",
                        "45   z := z + 1 ;",
                        "50   print z ; ",
                        "60   goto 40 ;",
                        "70 end ;",
                        "80 print 'done.' ;"
                    };
           MGCBasic mgc;
           mgc.runProgram(program);
           return 0;
     }
     max@MaxGorenLaptop:~/mgcpl$ g++ -g mgcpl.cpp
     max@MaxGorenLaptop:~/mgcpl$ ./a.out
     16 
     17 
     18 
     19 
     20 
     done.
     max@MaxGorenLaptop:/mnt/c/Users/mgoren/Desktop/pmpc/mgcpl$
