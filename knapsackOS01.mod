param n;
set V;
param capacity;
param weight{V};
param valeur{V};
var X{i in V} binary;

maximize val : sum{ i in 1..n} X[i]*valeur[i];

capacite : sum{i in 1..n} X[i]*weight[i]<=capacity;

solve;
display val;
data;
param n := 20;
param capacity := 250;
param: V : weight valeur :=
  1 9 73
  2 49 9
  3 17 16
  4 32 98
  5 29 61
  6 42 49
  7 14 13
  8 32 4
  9 25 56
  10 39 98
  11 50 1
  12 45 58
  13 18 93
  14 15 76
  15 7 41
  16 2 3
  17 2 84
  18 35 2
  19 25 88
  20 14 55;
end;
