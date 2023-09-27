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
65 end;