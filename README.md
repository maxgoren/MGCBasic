# MGCBasic
A *very* basic BASIC interpreter.

As it stands, code should be passed to the interpreter as a vector of strings.
I told you this was *very* basic.

Syntax is derived from the "Tiny BASIC" specification, which is so kind as to include a complete
BNF grammar, alot which i ignored, the rest of which i probably misunderstood.

Currently supported(i use the term loosely):
  - assign integers to variable names using 'let var := <number>'
  - yes, assignment is done with :=, my implementation my choice :D
  - you can use whatever line numbering scheme you want, but i suggest they be ordered and ascending.
  - print statements can print strings in single quotes, or variable names. You cant combine the two, and every print automatically inclues the new line automatically.
  - Loops are performed via goto. Take that Dijkstra! (Just kidding. Thanks for the shunting yard algorithm, i make use of it here) coincidentally, this is the only reason i included line numbering.
  - evaluate infix math expressions, expressions can intermix variable names with numbers (gasp!)
  
    
Currently not supported:
  - Clear, Run, List - this makes sense, because there is no REPL (brb, going to implement one)
  - Error handling
  - Lots of stuff actually. Pretty much anything not showin in the example program below
  - This project is somewhat tongue in cheek, and im surprised it works at all, let alone as it does.


  
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


"She might be primitive, But I'm still Damn Proud!"
