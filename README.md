# MGCBasic
A *very* basic BASIC interpreter.

It has a REPL BASIC environement, or you can pass code supplied as a vector of strings as shown below.
I told you this was *very* basic.

Syntax is *derived* from the "Tiny BASIC" specification(ahem, wikipedia page), which is so kind as to include a complete
BNF grammar, alot which i ignored, the rest of which i adapted poorly.

Currently supported(i use the term loosely):
  - assign integers to variable names using 'let var := <number>' (yes, assignment is done with :=, my implementation my choice :D)
  - variables can be updated without using 'let' once theyve been initialized
  - you can use whatever line numbering scheme you want, but i suggest they be ordered and ascending.
  - print statements can print strings in single quotes, or variable names. You cant combine the two, and every print automatically inclues the new line automatically.
  - Loops are performed via goto. Take that Dijkstra! (Just kidding. Thanks for the shunting yard algorithm, i make use of it here) coincidentally, this is the only reason i included line numbering.
  - evaluate infix math expressions, expressions can intermix variable names with numbers (gasp!)
  - REPL - this is more like an oldschool basic environment as youd find on the TRS-80, etc. than a modern REPL. for example, it doesnt evaluate when you hit enter, you have to specficially tell it to execute.
  
  - REPL Features:
    - enter lines of code one by one
    - can edit previously entered line by entering new line with same line number.
    - if you omit line numbers, they will be assigned automatically
    - .list will show you your program entered so far, including line numbers.
    - .run to execute the program you've entered
    - .done to exit the REPL
    


  
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
           mgc.REPL()
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
     > 10 let first := 10 ;
     > 20 let second := 40 ;
     > 30 if (first < second) then
     > 40 print 'it is!' ;
     > 50 end ;
     > .run
     it is!
     > .done
     max@MaxGorenLaptop:/mnt/c/Users/mgoren/Desktop/pmpc/mgcpl$


"She might be primitive, But I'm still Damn Proud!"
