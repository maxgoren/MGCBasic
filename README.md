# MGCBasic
A *very* basic BASIC interpreter.

It has a REPL BASIC environement, or you can pass code supplied as a vector of strings as shown below.
I told you this was *very* basic.

Syntax is *derived* from the "Tiny BASIC" specification(ahem, wikipedia page), which is so kind as to include a complete
BNF grammar, alot which i ignored, the rest of which i adapted poorly.

### Currently supported:
  - assign integers to variable names using 'let var := <number>' (yes, assignment is done with :=, my implementation my choice :D)
  - variables can be updated without using 'let' once theyve been initialized
  - you can use whatever line numbering scheme you want, but i suggest they be ordered and ascending.
  - print statements can print strings in single quotes, or variable names. every print statement inclues the new line automatically.
  - Loops are performed via goto. Take that Dijkstra! (Just kidding. Thanks for the shunting yard algorithm, i make use of it here.) coincidentally, this is the only reason i included line numbering.
  - evaluate infix math expressions, expressions can intermix variable names with numbers (gasp!)
  - REPL - this is more like an oldschool basic environment as youd find on the TRS-80, etc. than a modern REPL. for example, it doesnt evaluate when you hit enter, you have to specficially tell it to execute.
  
    ### REPL Features:
    - enter lines of code one by one
    - can edit previously entered line by entering new line with same line number.
    - if you omit line numbers, they will be assigned automatically
    - .list will show you your program entered so far, including line numbers.
    - .run to execute the program you've entered
    - .done to exit the REPL
    
 ### Using the REPL
 below is an example session showing the REPL in action, you can see that lines can be entered "out of order", and that
 omitted line numbers will be generated automatically.
     
     max@MaxGorenLaptop:~/mgcpl$ ./mgcpl
     MGCBasic 0.1b (c) 2023 maxgcoding.com
     -------------------------------------
     repl> let a:= 2 ;
     repl> 10 let a := 2 ;        
     repl> 20 let b := 3 ;
     repl> 30 print c ;
     repl> 25 let c := a + b ;
     repl> .list
     10 let a := 2 ;
     20 let b := 3 ;
     25 let c := a + b ;
     30 print c ;
     repl> .run
     5
     repl> .quit
     repl> .done
     max@MaxGorenLaptop:/mnt/c/Users/mgoren/Desktop/pmpc/mgcpl$

  
### example program using vector of strings
An example of supplying a program as a vector of strings. This makes it possible to use
MGCBasic as an embedded scripting language for your projects.
      
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
     max@MaxGorenLaptop:~/mgcpl$

### Still to come
 - print expressions
 - expressions besides relop expressions inside if statements
 - implement go sub, singline functions, and return
 - *who knows*

"She might be primitive, But I'm still Damn Proud!"
