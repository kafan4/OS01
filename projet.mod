param c;
param s;
param v;
param Ci{i in 1..s};
param T{i in 1..v,j in 1..s};

var X {i in 1..c,j in 1..s} binary;

minimize cout: sum {i in 1..c, j in 1..s} Ci[j]*X[i,j];

affectation {i in 1..c} : sum { j in 1..s} X[i,j]=1;
respectvol {k in 1..v}: sum {i in 1..c, j in 1..s} X[i,j]*T[k,j]>=1;


solve;
display cout,X;
data;
param c:=3;
param s:=12;
param v:=11;

param T : 1 2 3 4 5 6 7 8 9 10 11 12 := 
1 1 0 0 1 0 0 1 0 0 1 0 0
2 0 1 0 0 1 0 0 1 0 0 1 0
3 0 0 1 0 0 1 0 0 1 0 0 1
4 0 0 0 1 0 0 1 0 1 1 0 1
5 1 0 0 0 0 1 0 0 0 1 1 0
6 0 0 0 1 1 0 0 0 1 0 0 0
7 0 0 0 0 0 0 1 1 0 1 1 1
8 0 1 0 1 1 0 0 0 1 0 0 0
9 0 0 0 0 1 0 0 1 0 0 1 0
10 0 0 1 0 0 0 1 1 0 0 0 1 
11 0 0 0 0 0 1 0 0 1 1 1 1;

param Ci := 1 2 2 3 3 4 4 6 5 7 6 5 7 7 8 8 9 9 10 9 11 8 12 9;









