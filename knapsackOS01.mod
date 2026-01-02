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
end;
