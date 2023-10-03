10 dim i as num;
15 dim j as num;
20 i := 2;
25 i := 2;
30 j := 3;
35 let c := j;
40 i := i + 1;
45 j := j + i;
50 if (c < 100) then
55 println c;
60 goto 40;
65 end;
