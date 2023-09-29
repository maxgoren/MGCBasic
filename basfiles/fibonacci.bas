10 let prev := 0;
15 let curr := 1;
20 let fib := prev + curr;
25 for i := 1 to 10 step 1
30   prev := curr;
35   curr := fib;
40   print fib;
45   fib := prev + curr;
50 next i