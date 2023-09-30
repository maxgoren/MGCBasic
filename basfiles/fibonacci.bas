2 dim prev as num;
3 dim curr as num;
4 dim fib as num;
10 prev := 0;
15 curr := 1;
20 fib := prev + curr;
25 for i := 1 to 10 step 1
30   prev := curr;
35   curr := fib;
40   print fib;
45   fib := prev + curr;
50 next i;