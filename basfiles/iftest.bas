10 dim i as num;
15 dim j as num;
20 j := 1;
25 i := 3;
30 let k := i + j;
35 if (k < 37) then
40 j := i;
45 i := i + 1;
50 println k;
55 goto 30;
60 if (k > 0) then
65 k := k - 1;
70 println k;
75 goto 60;
80 end;
85 println 'strange';