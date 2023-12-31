# MGCBasic
An embeddable BASIC interpreter with REPL-like environment.

### About
MGC Basic is a simple interpreter for a dialet of the BASIC language. It has a REPL-like BASIC environement reminiscent of Level I BASIC on a TRS-80.
Using the REPL you can load a program from a saved .bas file. Alternatively, you can pass basic code supplied as a vector of strings to the interpreter as shown below.

The syntax is *derived* from the "Tiny BASIC" language specification(ahem, wikipedia page), which is so kind as to include a BNF grammar.
I did convert it to lowercase however, because TYPING LIKE THIS GETS OLD FAST. Features not present in Tiny BASIC such as for loop's have been added.

This project is still in its early stages and should not be considered "stable".

### Getting MGCBasic
This repository provides you with a make file, simple clone this repository to your computer
and in a unix/linux environment run.

     'make' 
     ./mgcbasic

MGCBasic has been tested and compiles on Linux and MacOS with both g++ and clang++,
as well as on windows with g++ and MSVC.

### Currently supported:
  - assign integers to variable names using 'let var := <number>' (yes, assignment is done with :=, my implementation my choice :D)
  - variables can be updated without using 'let' once theyve been initialized(declared variables must be initialized to a value at time of decleration.
  - print statements can print strings in single quotes, or variable names. every print statement inclues the new line automatically.
  - looping with for loops, or the older line number goto style looping.
  - control flow with if statements
  - evaluate infix math expressions, expressions can intermix variable names with numbers 
  - REPL - this is more like an oldschool basic environment as youd find on the TRS-80, etc. on early home computers, this was akin to the operating system.
  - input data into a pre-declared variable (integers only)
  
    ### REPL Features:
    - enter lines of code one by one
    - load source from file using '.load filename' 
    - can edit lines by entering new line with same line number as line you want changed.
    - if you omit line numbers, they will be assigned automatically (not when loading from file)
    - .list will show you your program entered so far, including line numbers.
    - .run to execute the program you've entered
    - .done, .exit, and .quit will all exit the REPL
    - .clear to clear all entered code
    - .tokens prints out the output from the lexer
    - .symbols prints out any created variables and there current values

  ### still to come (in progress)
   - array support
   - string support
   - print expressions, intermix variables in print statements
   - expressions besides relop expressions inside if statements
   - implement gosub, singleline functions, and return
   - *who knows*

    
 ### Using the REPL
 below is an example session showing the REPL in action, you can see that lines can be entered "out of order", and that
 omitted line numbers will be generated automatically.
     
     max@MaxGorenLaptop:~/mgcpl$ ./mgcpl
     MGCBasic 0.1b (c) 2023 maxgcoding.com
     -------------------------------------
     repl> let a:= 2;
     repl> 10 let a := 2;        
     repl> let b := 3;
     repl> print c;
     repl> 25 let c := a + b;
     repl> .list
     10 let a := 2;
     20 let b := 3;
     25 let c := a + b;
     30 print c;
     repl> .run
     5
     repl> .quit
     repl> .done
     max@MaxGorenLaptop:/mnt/c/Users/mgoren/Desktop/pmpc/mgcpl$ ./mgcbasic 
     MGCBasic 0.1b (c) 2023 maxgcoding.com
     -------------------------------------
     repl> .load fibonacci.bas
     fibonacci.bas: 
     repl> .list
     10 let prev := 0;
     15 let curr := 1;
     20 let next := curr + prev;
     25 print prev;
     30 print curr;
     35 prev := curr;
     40 curr := next;
     45 if (next < 50) then
     50 print next;
     55 next := curr + prev;
     60 goto 35;
     65 end
     repl> .run
     0 
     1
     1
     2
     3
     5
     8
     13
     21
     34
     repl> .done
     max@MaxGorenLaptop:/mnt/c/Users/mgoren/Desktop/pmpc/mgcpl$

### Loading & running programs from source file on the command line
Of course, you dont need to use the REPL to load and run a program from source, simply invoke the interpreter with the filename as the first argument
on the command line:
     
      max@MaxGorenLaptop:/mnt/c/Users/mgoren/Desktop/pmpc/mgcpl$ ./mgcbasic fibonacci.bas 
      0  
      1  
      1  
      2  
      3  
      5  
      8  
      13 
      21 
      34 
      max@MaxGorenLaptop:/mnt/c/Users/mgoren/Desktop/pmpc/mgcpl$
  
### Example program using vector of strings
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

### Debugging
There are some limited debugging capabilities to show how your code is being interpreted.

     max@MaxGorenLaptop:/mnt/c/Users/mgoren/Desktop/pmpc/mgcbasic$ ./mgcbasic
     MGCBasic 0.1b (c) 2023 maxgcoding.com
     repl> let a := 1;
     repl> .run 
     repl> .tokens
     <[  Token:   NUM,  line no: 10 , index:   0 - 2,  String:  10 ]>
     <[  Token:   LETSYM,  line no: 10,  index:   3 - 6,  String:  let ]>
     <[  Token:   IDSYM,  line no: 10,  index:   7 - 8,  String:  a ]>
     <[  Token:   ASSIGNSYM,  line no: 10,  index:   9 - 11,  String:  := ]>
     <[  Token:   NUM,  line no: 10,  index:   12 - 13,  String:  1 ]>
     <[  Token:   SEMICOLON,  line no: 10,  index:   13 - 14,  String:  ; ]>
     repl> .symbols
     a: 1
     repl> .exit
     max@MaxGorenLaptop:/mnt/c/Users/mgoren/Desktop/pmpc/mgcbasic$ 


"She might be primitive, But I'm still Damn Proud!"
